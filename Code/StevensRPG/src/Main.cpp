#include "Main.h"

// Declare our game instance
Main game;

Main::Main()
    : _scene(NULL)
{
}

void Main::initialize()
{
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

	//Setup sprite
	Node* spriteNode = Node::create();

	Sprite* sprite = Sprite::create("grass", _tilesheet);
	sprite->setDefaultTile(Rectangle(0, 0, 16, 16));
	sprite->setSpriteSize(16, 16);
	sprite->setSpriteOffset(-(sprite->getSpriteSize() * 0.5));

	spriteNode->setSprite(sprite);

	_scene->addNode(spriteNode);

	SAFE_RELEASE(sprite);
	SAFE_RELEASE(spriteNode);
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
}

void Main::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Main::drawScene);
}

bool Main::drawScene(Node* node)
{
	//Draw the sprite if it exists
	Sprite* sprite = node->getSprite();
	if(sprite)
	{
		sprite->draw();
	}
	return true;
}

void Main::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
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
