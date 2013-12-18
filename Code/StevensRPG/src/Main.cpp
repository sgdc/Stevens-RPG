#include "Main.h"

// Declare our game instance
Main game;

Main::Main()
    : _scene(NULL), _tilesheet(NULL), _cameraMovement()
{
}

void Main::setupAnimation(Sprite* sprite)
{
    //Size and offset
    unsigned int times[] = {0, 200, 400};
    float offset_values[6] = {sprite->getOffsetX(), sprite->getOffsetY()};
    offset_values[2] = offset_values[0] - ((sprite->getWidth() * 2) - sprite->getWidth()) / 2.0f;
    offset_values[3] = offset_values[1] - ((sprite->getHeight() * 2) - sprite->getHeight()) / 2.0f;
    offset_values[4] = offset_values[0];
    offset_values[5] = offset_values[1];

    sprite->createAnimation("animate_sprite_offset", Sprite::ANIMATE_OFFSET, 3, times, offset_values, Curve::SMOOTH)->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);

    //Indexed frames
    int index = sprite->getTileSheet()->getStripIndex("lava1");
    unsigned int animate_times[] = {0, 100, 200, 300, 400, 500, 600, 700, 800};
    float animate2_values[] = {
        index, 0,
        index, 1,
        index, 2,
        index, 3,
        index, 4,
        index, 3,
        index, 2,
        index, 1
    };
    
    sprite->createAnimation("animate_sprite_index", Sprite::ANIMATE_FRAME_INDEX, 9, animate_times, animate2_values, Curve::STEP)->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
}

/*void lookAt(Node* node, const Vector3& eye, const Vector3& target, const Vector3& up)
{
    //From: http://www.gameplay3d.org/forums/viewtopic.php?f=3&t=315

    // Create lookAt matrix
    Matrix matrix;
    Matrix::createLookAt(eye, target, up, &matrix);

    // Pull SRT components out of matrix
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    matrix.decompose(&scale, &rotation, &translation);

    // Set SRT on node
    node->setScale(scale);
    node->setTranslation(translation);
    node->setRotation(rotation);
}*/

void Main::initialize()
{
    //Set default sprite offset
    Sprite::setDefaultSpriteOffset(-(Vector2(getWidth(), getHeight()) * 0.5f));

    //Create the scene
    _scene = Scene::load("res/basic.scene");

    //lookAt(_scene->getActiveCamera()->getNode(), Vector3(-10, -100, -10), Vector3(), Vector3::unitY());

    SpriteGroup* map = static_cast<SpriteGroup*>(_scene->findNode("Map_Node")->getSprite());
    _tilesheet = map->getTileSheet();

    //Setup lava
    Sprite* sprite = map->getSprite(2, 2);
    setupAnimation(sprite);
    _aniSprite = sprite;
}

void Main::finalize()
{
    SAFE_RELEASE(_scene);
}

#define MOVEMENT_DELTA 1

void Main::update(float elapsedTime)
{
    //TODO
    if(!_cameraMovement.isZero() && elapsedTime != 0)
    {
        _scene->getActiveCamera()->getNode()->translate((_cameraMovement * (MOVEMENT_DELTA / _cameraMovement.length())) * elapsedTime);
    }
}

void Main::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _tilesheet->startBatch(_scene->getActiveCamera(), true);

    _scene->visit(this, &Main::drawScene);

    _tilesheet->finishBatch();
}

bool Main::drawScene(Node* node)
{
    //Draw the sprite if it exists
    Sprite* sprite = node->getSprite();
    if(sprite)
    {
        sprite->draw(sprite->getTileSheet() != _tilesheet);
    }
    return true;
}

void Main::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        Sprite* sprite;
        switch (key)
        {
            case Keyboard::KEY_ESCAPE:
                exit();
                break;
            case Keyboard::KEY_DOWN_ARROW:
                _cameraMovement.y -= MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_UP_ARROW:
                _cameraMovement.y += MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_LEFT_ARROW:
                _cameraMovement.x -= MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_RIGHT_ARROW:
                _cameraMovement.x += MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_R:
                _scene->visit(this, &Main::rotateLeft);
                break;
            case Keyboard::KEY_E:
                _scene->visit(this, &Main::rotateRight);
                break;
            case Keyboard::KEY_V:
                sprite = _scene->getFirstNode()->getNextSibling()->getSprite();
                if(sprite)
                {
                    if((sprite->getFlip() & Sprite::FLIP_VERT) == Sprite::FLIP_VERT)
                    {
                        sprite->setFlip(sprite->getFlip() & ~Sprite::FLIP_VERT);
                    }
                    else
                    {
                        sprite->setFlip(sprite->getFlip() | Sprite::FLIP_VERT);
                    }
                }
                break;
            case Keyboard::KEY_H:
                sprite = _scene->getFirstNode()->getNextSibling()->getSprite();
                if(sprite)
                {
                    if((sprite->getFlip() & Sprite::FLIP_HORZ) == Sprite::FLIP_HORZ)
                    {
                        sprite->setFlip(sprite->getFlip() & ~Sprite::FLIP_HORZ);
                    }
                    else
                    {
                        sprite->setFlip(sprite->getFlip() | Sprite::FLIP_HORZ);
                    }
                }
                break;
            case Keyboard::KEY_A:
                //sprite = _scene->getFirstNode()->getNextSibling()->getSprite();
                if(_aniSprite)
                {
                    _aniSprite->getAnimation("animate_sprite_index")->play();
                }
                break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
            case Keyboard::KEY_DOWN_ARROW:
                _cameraMovement.y += MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_UP_ARROW:
                _cameraMovement.y -= MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_LEFT_ARROW:
                _cameraMovement.x += MOVEMENT_DELTA;
                break;
            case Keyboard::KEY_RIGHT_ARROW:
                _cameraMovement.x -= MOVEMENT_DELTA;
                break;
        }
    }
}

bool Main::rotateLeft(Node* node)
{
    //Draw the sprite if it exists
    Sprite* sprite = node->getSprite();
    if(sprite)
    {
        node->rotateZ(MATH_DEG_TO_RAD(1.0f));
    }
    return true;
}

bool Main::rotateRight(Node* node)
{
    //Draw the sprite if it exists
    Sprite* sprite = node->getSprite();
    if(sprite)
    {
        node->rotateZ(MATH_DEG_TO_RAD(-1.0f));
    }
    return true;
}

void Main::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
        case Touch::TOUCH_PRESS:
            break;
        case Touch::TOUCH_RELEASE:
            break;
        case Touch::TOUCH_MOVE:
            break;
    };
}
