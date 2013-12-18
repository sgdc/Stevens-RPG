#ifndef Main_H_
#define Main_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class Main: public Game
{
public:

    /**
     * Constructor.
     */
    Main();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    void setupAnimation(Sprite* sprite);

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);

    bool rotateLeft(Node* node);
    bool rotateRight(Node* node);

    Scene* _scene;
    TileSheet* _tilesheet;
	int _cameraMovementKeys;
    Sprite* _aniSprite;
};

#endif
