#include "Main.h"

// Declare our game instance
Main game;

Main::Main()
	: _scene(NULL), _tilesheet(NULL), _cameraMovement()
{
}

void Main::setupAnimation(Sprite* sprite)
{
	//TODO
}

void Main::initialize()
{
	//Set default sprite offset
	Sprite::setDefaultSpriteOffset(-(Vector2(getWidth(), getHeight()) * 0.5f));

	//Create the scene
	_scene = Scene::create("game_scene");

	//Setup camera
	Node* camNode = Node::create("Cam_Node");
	Camera* cam = Camera::createOrthographic(getWidth(), getHeight(), getAspectRatio(), 0, 1);

	camNode->setCamera(cam);

	_scene->addNode(camNode);
	_scene->setActiveCamera(cam);

	SAFE_RELEASE(cam);
	SAFE_RELEASE(camNode);

	//Setup tilesheet
	Texture* tilesheetTex = Texture::create("res/textures/tilesheet-v1.png");
	_tilesheet = TileSheet::create("game_tileSheet", tilesheetTex);
	Texture::Sampler* sampler = _tilesheet->getSpriteBatch()->getSampler();
	sampler->setFilterMode(Texture::NEAREST, Texture::NEAREST);
	sampler->setWrapMode(Texture::REPEAT, Texture::REPEAT);

	//Setup sprite
	Node* spriteNode = Node::create();

	Sprite* sprite = Sprite::create("grass", _tilesheet);
	sprite->setDefaultTile(Rectangle(16, 16));
	sprite->setSize(64, 64);
	setupAnimation(sprite);

	spriteNode->setSprite(sprite);

	_scene->addNode(spriteNode);

	for(int i = 0; i < ceilf(getWidth() / sprite->getWidth()); i++)
	{
		Node* spriteNodeClone = spriteNode->clone();
		SAFE_RELEASE(spriteNode);
		spriteNode = spriteNodeClone;

		spriteNode->translate(sprite->getWidth(), 0, 0);

		_scene->addNode(spriteNode);
	}
	SAFE_RELEASE(spriteNode);
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(tilesheetTex);
}

void Main::finalize()
{
    SAFE_RELEASE(_scene);
	SAFE_RELEASE(_tilesheet);
}

void Main::update(float elapsedTime)
{
	//TODO
	if(!_cameraMovement.isZero() && elapsedTime != 0)
	{
		_scene->getActiveCamera()->getNode()->translate(_cameraMovement * elapsedTime);
	}
}

void Main::render(float elapsedTime)
{
    // Clear the color and depth buffers
	clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
	_tilesheet->getSpriteBatch()->start();

	_tilesheet->getSpriteBatch()->setProjectionMatrix(_scene->getActiveCamera()->getViewProjectionMatrix());

    _scene->visit(this, &Main::drawScene);

	_tilesheet->getSpriteBatch()->finish();
}

bool Main::drawScene(Node* node)
{
	//Draw the sprite if it exists
	Sprite* sprite = node->getSprite();
	if(sprite)
	{
		sprite->draw(false);
	}
	return true;
}

#define MOVEMENT_DELTA 1

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
		node->rotateZ(MATH_DEG_TO_RAD(10.0f));
	}
	return true;
}

bool Main::rotateRight(Node* node)
{
	//Draw the sprite if it exists
	Sprite* sprite = node->getSprite();
	if(sprite)
	{
		node->rotateZ(MATH_DEG_TO_RAD(-10.0f));
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
