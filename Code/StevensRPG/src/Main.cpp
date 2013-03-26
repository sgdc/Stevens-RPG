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
	float size_values[] = {64, 64, 128, 128, 64, 64};
	float offset_values[6] = {sprite->getOffsetX(), sprite->getOffsetY()};
	offset_values[2] = offset_values[0] - (size_values[2] - size_values[0]) / 2.0f;
	offset_values[3] = offset_values[1] - (size_values[3] - size_values[1]) / 2.0f;
	offset_values[4] = offset_values[0];
	offset_values[5] = offset_values[1];

	sprite->createAnimation("animate_sprite_size", Sprite::ANIMATE_SIZE, 3, times, size_values, Curve::SMOOTH)->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);

	sprite->createAnimation("animate_sprite_offset", Sprite::ANIMATE_OFFSET, 3, times, offset_values, Curve::SMOOTH)->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);

	//Specific frames
	unsigned int animate_times[] = {0, 100, 200, 300, 400, 500, 600, 700, 800};
	float animate1_values[] = {
		96, 0, 16, 16,
		112, 0, 16, 16,
		128, 0, 16, 16,
		144, 0, 16, 16,
		160, 0, 16, 16,
		144, 0, 16, 16,
		128, 0, 16, 16,
		112, 0, 16, 16
	};
	sprite->createAnimation("animate_sprite_specific", Sprite::ANIMATE_FRAME_SPECIFIC, 9, animate_times, animate1_values, Curve::STEP)->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);

	//Indexed frames
	TileSheet* tileSheet = sprite->getTileSheet();
	unsigned int index = tileSheet->addStrip("lava1", 5);
	tileSheet->setStripFrame(index, 0, Rectangle(96, 0, 16, 16));
	tileSheet->setStripFrame(index, 1, Rectangle(112, 0, 16, 16));
	tileSheet->setStripFrame(index, 2, Rectangle(128, 0, 16, 16));
	tileSheet->setStripFrame(index, 3, Rectangle(144, 0, 16, 16));
	tileSheet->setStripFrame(index, 4, Rectangle(160, 0, 16, 16));

	index = tileSheet->addStrip("lava2", 5);
	tileSheet->setStripFrame(index, 0, Rectangle(176, 0, 16, 16));
	tileSheet->setStripFrame(index, 1, Rectangle(192, 0, 16, 16));
	tileSheet->setStripFrame(index, 2, Rectangle(208, 0, 16, 16));
	tileSheet->setStripFrame(index, 3, Rectangle(224, 0, 16, 16));
	tileSheet->setStripFrame(index, 4, Rectangle(240, 0, 16, 16));

#define ANIMATE_INDEX 0

	float animate2_values[] = {
		ANIMATE_INDEX, 0,
		ANIMATE_INDEX, 1,
		ANIMATE_INDEX, 2,
		ANIMATE_INDEX, 3,
		ANIMATE_INDEX, 4,
		ANIMATE_INDEX, 3,
		ANIMATE_INDEX, 2,
		ANIMATE_INDEX, 1
	};

	sprite->createAnimation("animate_sprite_index", Sprite::ANIMATE_FRAME_INDEX, 9, animate_times, animate2_values, Curve::STEP)->getClip()->setRepeatCount(AnimationClip::REPEAT_INDEFINITE);
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

	//Setup map
	Node* spriteNode = Node::create();

	SpriteGroup* map = SpriteGroup::create("grass_map", 100, 100, _tilesheet);
	map->setDefaultTile(Rectangle(16, 16));
	map->setSize(64, 64);
	//map->setHorzGap(10);
	//map->setVertGap(10);
	//setupAnimation(static_cast<Sprite*>(map));

	spriteNode->setSprite(static_cast<Sprite*>(map));

	_scene->addNode(spriteNode);
	
	//Setup lava
	Sprite* sprite = Sprite::create("lava", _tilesheet);
	_aniSprite = sprite;
	sprite->setSize(64, 64);
	setupAnimation(sprite);
	sprite->setDefaultTile(_tilesheet->getStripFrame("lava1", 1));

	map->setSpriteType(3, 3, SpriteGroup::TYPE_TRANSPARENT);
	map->setSpriteType(3, 4, SpriteGroup::TYPE_TRANSPARENT);
	map->setSpriteType(4, 3, SpriteGroup::TYPE_TRANSPARENT);
	map->setSpriteType(4, 4, SpriteGroup::TYPE_TRANSPARENT);
	map->setSprite(2, 2, sprite);
	map->setSprite(2, 3, sprite);
	map->setSprite(2, 4, sprite);
	map->setSprite(3, 2, sprite);
	map->setSprite(4, 2, sprite);

	SAFE_RELEASE(sprite);
	SAFE_RELEASE(map);

	/*Sprite* sprite = Sprite::create("grass", _tilesheet);
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
	SAFE_RELEASE(sprite);*/
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
