#include "Examples.h"
#include "SamplesGame.h"

const Game::State& Examples::UNINITIALIZED = Game::UNINITIALIZED;
const Game::State& Examples::RUNNING = Game::RUNNING;
const Game::State& Examples::PAUSED = Game::PAUSED;

const Game::ClearFlags& Examples::CLEAR_COLOR = Game::CLEAR_COLOR;
const Game::ClearFlags& Examples::CLEAR_DEPTH = Game::CLEAR_DEPTH;
const Game::ClearFlags& Examples::CLEAR_STENCIL = Game::CLEAR_STENCIL;
const Game::ClearFlags& Examples::CLEAR_COLOR_DEPTH = Game::CLEAR_COLOR_DEPTH;
const Game::ClearFlags& Examples::CLEAR_COLOR_STENCIL = Game::CLEAR_COLOR_STENCIL;
const Game::ClearFlags& Examples::CLEAR_DEPTH_STENCIL = Game::CLEAR_DEPTH_STENCIL;
const Game::ClearFlags& Examples::CLEAR_COLOR_DEPTH_STENCIL = Game::CLEAR_COLOR_DEPTH_STENCIL;

Examples::Examples()
{
}

Examples::~Examples()
{
	enableScriptCamera(false);
}

bool Examples::isVsync()
{
    return Game::isVsync();
}

void Examples::setVsync(bool enable)
{
    Game::setVsync(enable);
}

long Examples::getAbsoluteTime()
{
    return Game::getAbsoluteTime();
}

long Examples::getGameTime()
{
    return Game::getGameTime();
}

Game::State Examples::getState() const
{
    return Game::getInstance()->getState();
}

int Examples::run()
{
    return Game::getInstance()->run();
}

void Examples::pause()
{
    Game::getInstance()->pause();
}

void Examples::resume()
{
    Game::getInstance()->resume();
}

void Examples::exit()
{
    Game::getInstance()->exit();
}

void Examples::frame()
{
    Game::getInstance()->frame();
}

unsigned int Examples::getFrameRate() const
{
    return Game::getInstance()->getFrameRate();
}

const Rectangle& Examples::getViewport() const
{
    return Game::getInstance()->getViewport();
}

void Examples::setViewport(const Rectangle& viewport)
{
	Game::getInstance()->setViewport(viewport);
}

unsigned int Examples::getWidth() const
{
    return Game::getInstance()->getWidth();
}

unsigned int Examples::getHeight() const
{
    return Game::getInstance()->getHeight();
}

float Examples::getAspectRatio() const
{
    return Game::getInstance()->getAspectRatio();
}

void Examples::clear(Game::ClearFlags flags, const Vector4& clearColor, float clearDepth, int clearStencil)
{
    Game::getInstance()->clear(flags, clearColor, clearDepth, clearStencil);
}

void Examples::clear(Game::ClearFlags flags, float red, float green, float blue, float alpha, float clearDepth, int clearStencil)
{
    Game::getInstance()->clear(flags, red, green, blue, alpha, clearDepth, clearStencil);
}

AudioController* Examples::getAudioController() const
{
    return Game::getInstance()->getAudioController();
}

AnimationController* Examples::getAnimationController() const
{
    return Game::getInstance()->getAnimationController();
}

PhysicsController* Examples::getPhysicsController() const
{
    return Game::getInstance()->getPhysicsController();
}

ScriptController* Examples::getScriptController() const
{
	return Game::getInstance()->getScriptController();
}

void Examples::displayKeyboard(bool display)
{
    Game::getInstance()->displayKeyboard(display);
}

void Examples::keyEvent(Keyboard::KeyEvent evt, int key)
{    
}

void Examples::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
}

bool Examples::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    return false;
}

bool Examples::hasMouse()
{
    return Game::getInstance()->hasMouse();
}

bool Examples::isMouseCaptured()
{
    return Game::getInstance()->isMouseCaptured();
}

void Examples::setMouseCaptured(bool captured)
{
    Game::getInstance()->setMouseCaptured(captured);
}

void Examples::setMultiTouch(bool enabled)
{
    Game::getInstance()->setMultiTouch(enabled);
}

bool Examples::isMultiTouch() const
{
    return Game::getInstance()->isMultiTouch();
}

bool Examples::hasAccelerometer() const
{
    return Game::getInstance()->hasAccelerometer();
}

void Examples::getAccelerometerValues(float* pitch, float* roll)
{
    Game::getInstance()->getAccelerometerValues(pitch, roll);
}

void Examples::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    Game::getInstance()->getSensorValues(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
}

void Examples::schedule(long timeOffset, TimeListener* timeListener, void* cookie)
{
    Game::getInstance()->schedule(timeOffset, timeListener, cookie);
}

void Examples::enableScriptCamera(bool enable)
{
	Game::getInstance()->getScriptController()->executeFunction<void>("camera_setActive", "b", NULL, enable);
}

void Examples::setScriptCameraSpeed(float normal, float fast)
{
    Game::getInstance()->getScriptController()->executeFunction<void>("camera_setSpeed", "ff", NULL, normal, fast);
}

bool Examples::isGestureSupported(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureSupported(evt);
}

void Examples::registerGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->registerGesture(evt);
}

void Examples::unregisterGesture(Gesture::GestureEvent evt)
{
    Game::getInstance()->unregisterGesture(evt);
}

bool Examples::isGestureRegistered(Gesture::GestureEvent evt)
{
    return Game::getInstance()->isGestureRegistered(evt);
}

void Examples::gestureSwipeEvent(int x, int y, int direction)
{
}

void Examples::gesturePinchEvent(int x, int y, float scale)
{
}

void Examples::gestureTapEvent(int x, int y)
{
}

void Examples::gestureLongTapEvent(int x, int y, float duration)
{
}

void Examples::gestureDragEvent(int x, int y)
{
}

void Examples::gestureDropEvent(int x, int y)
{
}

void Examples::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
}

unsigned int Examples::getGamepadCount() const
{
    return Game::getInstance()->getGamepadCount();
}

Gamepad* Examples::getGamepad(unsigned int index, bool preferPhysical) const
{
    return Game::getInstance()->getGamepad(index, preferPhysical);
}

void Examples::drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps)
{
    char buffer[10];
    sprintf(buffer, "%u", fps);
    font->start();
    font->drawText(buffer, x, y, color, 18);
    font->finish();
}
