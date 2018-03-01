#include "Sponza.h"
#include "SamplesGame.h"
#include "BGFX/BGFXRenderer.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Graphics", "Sponza", Sponza, 5);
#endif

static const unsigned int MOVE_FORWARD = 1;
static const unsigned int MOVE_BACKWARD = 2;
static const unsigned int MOVE_LEFT = 4;
static const unsigned int MOVE_RIGHT = 8;
static const unsigned int MOVE_UP = 16;
static const unsigned int MOVE_DOWN = 32;
static const float MOVE_SPEED = 15.0f;
static const float UP_DOWN_SPEED = 10.0f;

Sponza::Sponza()
    : _font(NULL)
    , _myModel(NULL)
    , _spinDirection(-1.0f)
    , _moveFlags(0), _prevX(0), _prevY(0), _buttonPressed(false)
{    
}

void Sponza::initialize()
{
    // Create the font for drawing the framerate.
    _font = Font::create("res/ui/arial.gpb");

    _scene = Scene::load("res/common/sponza/sponza.gpb");

    // set fps camera
    Vector3 cameraPosition(0, 1, 10);
    _fpCamera.initialize(1.0, 100000.0f);
    _fpCamera.setPosition(cameraPosition);
    _scene->addNode(_fpCamera.getRootNode());
    _scene->setActiveCamera(_fpCamera.getCamera());

    // Update the aspect ratio for our scene's camera to match the current device resolution
    _scene->getActiveCamera()->setAspectRatio(getAspectRatio());
}

void Sponza::finalize()
{
    SAFE_RELEASE(_myModel);
    SAFE_RELEASE(_font);
}

void Sponza::update(float elapsedTime)
{
    updateCamera(elapsedTime);
}

void Sponza::render(float elapsedTime)
{
    _scene->visit(this, &Sponza::drawScene);

    // draw frame rate
    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());
}

bool Sponza::drawScene(Node* node)
{
    Drawable* drawable = node->getDrawable();
    if (drawable)
        drawable->draw();
    return true;
}

void Sponza::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
    {
        if (x < 75 && y < 50)
        {
            // Toggle Vsync if the user touches the top left corner
            setVsync(!isVsync());
        }
        _prevX = x;
        _prevY = y;
        break;
    }
    case Touch::TOUCH_RELEASE:
    {
        _prevX = 0;
        _prevY = 0;
        break;
     }
    case Touch::TOUCH_MOVE:
    {
        int deltaX = x - _prevX;
        int deltaY = y - _prevY;
        _prevX = x;
        _prevY = y;
        float pitch = -MATH_DEG_TO_RAD(deltaY * 0.5f);
        float yaw = MATH_DEG_TO_RAD(deltaX * 0.5f);
        _fpCamera.rotate(yaw, pitch);
        break;
    }
    };
}

void Sponza::updateCamera(float elapsedTime)
{
    float time = (float)elapsedTime / 1000.0f;

    Vector2 move;

    if (_moveFlags != 0)
    {
        // Forward motion
        if (_moveFlags & MOVE_FORWARD)
        {
            move.y = 1;
        }
        else if (_moveFlags & MOVE_BACKWARD)
        {
            move.y = -1;
        }
        // Strafing
        if (_moveFlags & MOVE_LEFT)
        {
            move.x = 1;
        }
        else if (_moveFlags & MOVE_RIGHT)
        {
            move.x = -1;
        }
        move.normalize();

        // Up and down
        if (_moveFlags & MOVE_UP)
        {
            _fpCamera.moveUp(time * UP_DOWN_SPEED);
        }
        else if (_moveFlags & MOVE_DOWN)
        {
            _fpCamera.moveDown(time * UP_DOWN_SPEED);
        }
    }
    /*else if (_gamepad->getJoystickCount() > 0)
    {
        _gamepad->getJoystickValues(0, &move);
        move.x = -move.x;
    }
    if (_gamepad->getJoystickCount() > 1)
    {
        Vector2 joy2;
        _gamepad->getJoystickValues(1, &joy2);
        _fpCamera.rotate(MATH_DEG_TO_RAD(joy2.x * 2.0f), MATH_DEG_TO_RAD(joy2.y * 2.0f));
    }*/

    if (!move.isZero())
    {
        move.scale(time * MOVE_SPEED);
        _fpCamera.moveForward(move.y);
        _fpCamera.moveLeft(move.x);
    }

    /*if (!_buttonPressed && _gamepad->isButtonDown(Gamepad::BUTTON_A))
    {
        addSound("footsteps.wav");
    }
    _buttonPressed = _gamepad->isButtonDown(Gamepad::BUTTON_A);*/
}



void Sponza::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            _moveFlags |= MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
            _moveFlags |= MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
            _moveFlags |= MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
            _moveFlags |= MOVE_RIGHT;
            break;

        case Keyboard::KEY_Q:
            _moveFlags |= MOVE_DOWN;
            break;
        case Keyboard::KEY_E:
            _moveFlags |= MOVE_UP;
            break;
        case Keyboard::KEY_PG_UP:
            _fpCamera.rotate(0, MATH_PIOVER4);
            break;
        case Keyboard::KEY_PG_DOWN:
            _fpCamera.rotate(0, -MATH_PIOVER4);
            break;
        }
    }
    else if (evt == Keyboard::KEY_RELEASE)
    {
        switch (key)
        {
        case Keyboard::KEY_W:
            _moveFlags &= ~MOVE_FORWARD;
            break;
        case Keyboard::KEY_S:
            _moveFlags &= ~MOVE_BACKWARD;
            break;
        case Keyboard::KEY_A:
            _moveFlags &= ~MOVE_LEFT;
            break;
        case Keyboard::KEY_D:
            _moveFlags &= ~MOVE_RIGHT;
            break;
        case Keyboard::KEY_Q:
            _moveFlags &= ~MOVE_DOWN;
            break;
        case Keyboard::KEY_E:
            _moveFlags &= ~MOVE_UP;
            break;
        }
    }
}

bool Sponza::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
    case Mouse::MOUSE_WHEEL:
        _fpCamera.moveForward(wheelDelta * MOVE_SPEED / 2.0f );
        return true;
    }
    return false;
}


