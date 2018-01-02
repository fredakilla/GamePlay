#ifndef TestSample_H_
#define TestSample_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class TestSample: public Game
{
public:

    /**
     * Constructor.
     */
    TestSample();

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

    bool drawScene(Node* node);

    Model* _model;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
    Scene* _scene;

};

#endif
