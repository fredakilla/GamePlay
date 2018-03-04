#include "Base.h"
#include "Platform.h"
#include "Game.h"

#include <SDL2/SDL_config.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/platform.h>

#include "Renderer.h"

int __argc = 0;
char** __argv = 0;


namespace gameplay {

// sdl window
static SDL_Window * __window;
static int __windowSize[2];

// timer
static double __timeAbsolute;
static std::chrono::time_point<std::chrono::high_resolution_clock> __timeStart;

// mouse input
static bool __mouseCaptured = false;
static float __mouseCapturePointX = 0;
static float __mouseCapturePointY = 0;
static bool __cursorVisible = true;

// keys
uint8_t _translateKey[256];

// mobile device gesture
static bool __multiTouch = false;



extern int strcmpnocase(const char* s1, const char* s2)
{
#ifdef GP_PLATFORM_LINUX
    return strcasecmp(s1, s2);
#elif GP_PLATFORM_WINDOWS
    return _strcmpi(s1, s2);
#elif GP_PLATFORM_MACOS
    return strcasecmp(s1, s2);
#elif GP_PLATFORM_ANDROID
    return strcasecmp(s1, s2);
#elif GP_PLATFORM_IOS
    return strcasecmp(s1, s2);
#else
    GP_ERROR("Fix me !");
    return 0;
#endif
}

extern void print(const char* format, ...)
{
#if defined (GP_PLATFORM_WINDOWS)
    va_list argptr;
    va_start(argptr, format);
    int sz = vfprintf(stderr, format, argptr);
    if (sz > 0)
    {
        char* buf = new char[sz + 1];
        vsprintf(buf, format, argptr);
        buf[sz] = 0;
        OutputDebugStringA(buf);
        GP_SAFE_DELETE_ARRAY(buf);
    }
    va_end(argptr);
#elif defined (GP_PLATFORM_LINUX) || defined (GP_PLATFORM_MACOS)
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
#endif
}

void updateWindowSize()
{
    GP_ASSERT(__window);

    int width;
    int height;
    SDL_GetWindowSize(__window, &width, &height);

    Renderer::getInstance().updateWindowSize(width, height);

    __windowSize[0] = width;
    __windowSize[1] = height;
}



inline bool sdlSetWindow(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
    {
        return false;
    }

    bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.ndt          = wmi.info.x11.display;
    pd.nwh          = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
    pd.ndt          = wmi.info.vivante.display;
    pd.nwh          = wmi.info.vivante.window;
#endif // BX_PLATFORM_
    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    return true;
}




void initTranslateKey(uint16_t sdl, Keyboard::Key key)
{
    _translateKey[sdl & 0xff] = (uint8_t)key;
}

Keyboard::Key translateKey(SDL_Scancode sdl)
{
    return (Keyboard::Key)_translateKey[sdl & 0xff];
}






Platform::Platform(Game* game) :
    _game(game)
{

    std::memset(_translateKey, 0, sizeof(_translateKey));
    initTranslateKey(SDL_SCANCODE_ESCAPE,       Keyboard::KEY_ESCAPE);
    initTranslateKey(SDL_SCANCODE_RETURN,       Keyboard::KEY_RETURN);
    initTranslateKey(SDL_SCANCODE_TAB,          Keyboard::KEY_TAB);
    initTranslateKey(SDL_SCANCODE_BACKSPACE,    Keyboard::KEY_BACKSPACE);
    initTranslateKey(SDL_SCANCODE_SPACE,        Keyboard::KEY_SPACE);
    initTranslateKey(SDL_SCANCODE_UP,           Keyboard::KEY_UP_ARROW);
    initTranslateKey(SDL_SCANCODE_DOWN,         Keyboard::KEY_DOWN_ARROW);
    initTranslateKey(SDL_SCANCODE_LEFT,         Keyboard::KEY_LEFT_ARROW);
    initTranslateKey(SDL_SCANCODE_RIGHT,        Keyboard::KEY_RIGHT_ARROW);
    initTranslateKey(SDL_SCANCODE_PAGEUP,       Keyboard::KEY_PG_UP);
    initTranslateKey(SDL_SCANCODE_PAGEDOWN,     Keyboard::KEY_PG_DOWN);
    initTranslateKey(SDL_SCANCODE_HOME,         Keyboard::KEY_HOME);
    initTranslateKey(SDL_SCANCODE_END,          Keyboard::KEY_END);
    initTranslateKey(SDL_SCANCODE_PRINTSCREEN,  Keyboard::KEY_PRINT);
    initTranslateKey(SDL_SCANCODE_KP_PLUS,      Keyboard::KEY_PLUS);
    initTranslateKey(SDL_SCANCODE_EQUALS,       Keyboard::KEY_PLUS);
    initTranslateKey(SDL_SCANCODE_KP_MINUS,     Keyboard::KEY_MINUS);
    initTranslateKey(SDL_SCANCODE_MINUS,        Keyboard::KEY_MINUS);
    initTranslateKey(SDL_SCANCODE_GRAVE,        Keyboard::KEY_TILDE);
    initTranslateKey(SDL_SCANCODE_KP_COMMA,     Keyboard::KEY_COMMA);
    initTranslateKey(SDL_SCANCODE_COMMA,        Keyboard::KEY_COMMA);
    initTranslateKey(SDL_SCANCODE_KP_PERIOD,    Keyboard::KEY_PERIOD);
    initTranslateKey(SDL_SCANCODE_PERIOD,       Keyboard::KEY_PERIOD);
    initTranslateKey(SDL_SCANCODE_SLASH,        Keyboard::KEY_SLASH);
    initTranslateKey(SDL_SCANCODE_F1,           Keyboard::KEY_F1);
    initTranslateKey(SDL_SCANCODE_F2,           Keyboard::KEY_F2);
    initTranslateKey(SDL_SCANCODE_F3,           Keyboard::KEY_F3);
    initTranslateKey(SDL_SCANCODE_F4,           Keyboard::KEY_F4);
    initTranslateKey(SDL_SCANCODE_F5,           Keyboard::KEY_F5);
    initTranslateKey(SDL_SCANCODE_F6,           Keyboard::KEY_F6);
    initTranslateKey(SDL_SCANCODE_F7,           Keyboard::KEY_F7);
    initTranslateKey(SDL_SCANCODE_F8,           Keyboard::KEY_F8);
    initTranslateKey(SDL_SCANCODE_F9,           Keyboard::KEY_F9);
    initTranslateKey(SDL_SCANCODE_F10,          Keyboard::KEY_F10);
    initTranslateKey(SDL_SCANCODE_F11,          Keyboard::KEY_F11);
    initTranslateKey(SDL_SCANCODE_F12,          Keyboard::KEY_F12);
    initTranslateKey(SDL_SCANCODE_KP_0,         Keyboard::KEY_KP_INSERT);
    initTranslateKey(SDL_SCANCODE_KP_1,         Keyboard::KEY_KP_END);
    initTranslateKey(SDL_SCANCODE_KP_2,         Keyboard::KEY_KP_DOWN);
    initTranslateKey(SDL_SCANCODE_KP_3,         Keyboard::KEY_KP_PG_DOWN);
    initTranslateKey(SDL_SCANCODE_KP_4,         Keyboard::KEY_KP_LEFT);
    initTranslateKey(SDL_SCANCODE_KP_5,         Keyboard::KEY_KP_FIVE);
    initTranslateKey(SDL_SCANCODE_KP_6,         Keyboard::KEY_KP_RIGHT);
    initTranslateKey(SDL_SCANCODE_KP_7,         Keyboard::KEY_KP_HOME);
    initTranslateKey(SDL_SCANCODE_KP_8,         Keyboard::KEY_KP_UP);
    initTranslateKey(SDL_SCANCODE_KP_9,         Keyboard::KEY_PG_UP);
    initTranslateKey(SDL_SCANCODE_0,            Keyboard::KEY_ZERO);
    initTranslateKey(SDL_SCANCODE_1,            Keyboard::KEY_ONE);
    initTranslateKey(SDL_SCANCODE_2,            Keyboard::KEY_TWO);
    initTranslateKey(SDL_SCANCODE_3,            Keyboard::KEY_THREE);
    initTranslateKey(SDL_SCANCODE_4,            Keyboard::KEY_FOUR);
    initTranslateKey(SDL_SCANCODE_5,            Keyboard::KEY_FIVE);
    initTranslateKey(SDL_SCANCODE_6,            Keyboard::KEY_SIX);
    initTranslateKey(SDL_SCANCODE_7,            Keyboard::KEY_SEVEN);
    initTranslateKey(SDL_SCANCODE_8,            Keyboard::KEY_EIGHT);
    initTranslateKey(SDL_SCANCODE_9,            Keyboard::KEY_NINE);
    initTranslateKey(SDL_SCANCODE_A,            Keyboard::KEY_A);
    initTranslateKey(SDL_SCANCODE_B,            Keyboard::KEY_B);
    initTranslateKey(SDL_SCANCODE_C,            Keyboard::KEY_C);
    initTranslateKey(SDL_SCANCODE_D,            Keyboard::KEY_D);
    initTranslateKey(SDL_SCANCODE_E,            Keyboard::KEY_E);
    initTranslateKey(SDL_SCANCODE_F,            Keyboard::KEY_F);
    initTranslateKey(SDL_SCANCODE_G,            Keyboard::KEY_G);
    initTranslateKey(SDL_SCANCODE_H,            Keyboard::KEY_H);
    initTranslateKey(SDL_SCANCODE_I,            Keyboard::KEY_I);
    initTranslateKey(SDL_SCANCODE_J,            Keyboard::KEY_J);
    initTranslateKey(SDL_SCANCODE_K,            Keyboard::KEY_K);
    initTranslateKey(SDL_SCANCODE_L,            Keyboard::KEY_L);
    initTranslateKey(SDL_SCANCODE_M,            Keyboard::KEY_M);
    initTranslateKey(SDL_SCANCODE_N,            Keyboard::KEY_N);
    initTranslateKey(SDL_SCANCODE_O,            Keyboard::KEY_O);
    initTranslateKey(SDL_SCANCODE_P,            Keyboard::KEY_P);
    initTranslateKey(SDL_SCANCODE_Q,            Keyboard::KEY_Q);
    initTranslateKey(SDL_SCANCODE_R,            Keyboard::KEY_R);
    initTranslateKey(SDL_SCANCODE_S,            Keyboard::KEY_S);
    initTranslateKey(SDL_SCANCODE_T,            Keyboard::KEY_T);
    initTranslateKey(SDL_SCANCODE_U,            Keyboard::KEY_U);
    initTranslateKey(SDL_SCANCODE_V,            Keyboard::KEY_V);
    initTranslateKey(SDL_SCANCODE_W,            Keyboard::KEY_W);
    initTranslateKey(SDL_SCANCODE_X,            Keyboard::KEY_X);
    initTranslateKey(SDL_SCANCODE_Y,            Keyboard::KEY_Y);
    initTranslateKey(SDL_SCANCODE_Z,            Keyboard::KEY_Z);
}

Platform::~Platform()
{
}

Platform* Platform::create(Game* game)
{
    GP_ASSERT(game);

    FileSystem::setResourcePath("./");
    Platform* platform = new Platform(game);

    // Get the window configuration values
    const char *title = NULL;
    int __x = 0, __y = 0, __width = 1280, __height = 800, __samples = 0;
    bool fullscreen = false;
    if (game->getConfig())
    {
        Properties* config = game->getConfig()->getNamespace("window", true);
        if (config)
        {
            // Read window title.
            title = config->getString("title");

            // Read window rect.
            int x = config->getInt("x");
            int y = config->getInt("y");
            int width = config->getInt("width");
            int height = config->getInt("height");
            int samples = config->getInt("samples");
            fullscreen = config->getBool("fullscreen");

            if (fullscreen && width == 0 && height == 0)
            {
                // Use the screen resolution if fullscreen is true but width and height were not set in the config
                SDL_DisplayMode displayMode;
                SDL_GetCurrentDisplayMode(0, &displayMode);
                width = displayMode.w;
                height = displayMode.h;

            }
            if (x != 0) __x = x;
            if (y != 0) __y = y;
            if (width != 0) __width = width;
            if (height != 0) __height = height;
            if (samples != 0) __samples = samples;
        }
    }

    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        print("Failed to init SDL\n");
        return 0;
    }

    // Create our window centered
    __window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  __width,
                                  __height,
                                  SDL_WINDOW_SHOWN);

    // Check that everything worked out okay
    if (!__window)
    {
        print("Unable to create window\n");
        return false;
    }

    // init bgfx

    bgfx::RendererType::Enum supportedTypes[bgfx::RendererType::Count];
    uint8_t count =  bgfx::getSupportedRenderers(bgfx::RendererType::Count, supportedTypes);
    for(uint8_t i=0; i<count; i++)
        print("supported type [%d] = %s\n", i, bgfx::getRendererName(supportedTypes[i]));

    sdlSetWindow(__window);

    bgfx::init(bgfx::RendererType::OpenGL);


    uint32_t debug = BGFX_DEBUG_TEXT;
    uint32_t reset = BGFX_RESET_VSYNC;
    bgfx::reset(__width, __height, reset);

    // Enable debug text.
    bgfx::setDebug(debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , 0x552233ff
                       , 1.0f
                       , 0
                       );


    BGFXRenderer::initInstance();
    Renderer::getInstance().queryCaps();

    game->setVsync(true);
    game->setViewport(Rectangle(0,0,__width,__height));

    updateWindowSize();

    return platform;
}

int Platform::enterMessagePump()
{
    GP_ASSERT(_game);

    // Get the initial time.
    __timeStart = std::chrono::high_resolution_clock::now();
    __timeAbsolute = 0L;

    // Run the game.
    _game->run();

    bool loop = true;
    SDL_Event evt;

    while (loop)
    {
        if (_game)
        {
            Renderer::getInstance().beginFrame();
            _game->frame();
            Renderer::getInstance().endFrame();
        }

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
                case SDL_QUIT:
                {
                    _game->exit();
                    loop = false;
                }
                break;

                case SDL_MOUSEWHEEL:
                {
                    const SDL_MouseWheelEvent& wheelEvent = evt.wheel;
                    int wheelDelta = wheelEvent.y;
                    gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_WHEEL, 0, 0, wheelDelta);
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    gameplay::Mouse::MouseEvent mouseEvt;
                    const SDL_MouseButtonEvent& sdlMouseEvent = evt.button;

                    switch (sdlMouseEvent.button)
                    {
                    case SDL_BUTTON_LEFT:
                        mouseEvt = gameplay::Mouse::MOUSE_PRESS_LEFT_BUTTON;
                        break;
                    case SDL_BUTTON_RIGHT:
                        mouseEvt = gameplay::Mouse::MOUSE_PRESS_RIGHT_BUTTON;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        mouseEvt = gameplay::Mouse::MOUSE_PRESS_MIDDLE_BUTTON;
                        break;
                    }

                    if (!gameplay::Platform::mouseEventInternal(mouseEvt, sdlMouseEvent.x, sdlMouseEvent.y, 0))
                    {
                        gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_PRESS, sdlMouseEvent.x, sdlMouseEvent.y, 0, true);
                    }
                }
                break;

                case SDL_MOUSEBUTTONUP:
                {
                    gameplay::Mouse::MouseEvent mouseEvt;
                    const SDL_MouseButtonEvent& sdlMouseEvent = evt.button;

                    switch (sdlMouseEvent.button)
                    {
                    case SDL_BUTTON_LEFT:
                        mouseEvt = gameplay::Mouse::MOUSE_RELEASE_LEFT_BUTTON;
                        break;
                    case SDL_BUTTON_RIGHT:
                        mouseEvt = gameplay::Mouse::MOUSE_RELEASE_RIGHT_BUTTON;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        mouseEvt = gameplay::Mouse::MOUSE_RELEASE_MIDDLE_BUTTON;
                        break;
                    }

                    if (!gameplay::Platform::mouseEventInternal(mouseEvt, sdlMouseEvent.x, sdlMouseEvent.y, 0))
                    {
                        gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, sdlMouseEvent.x, sdlMouseEvent.y, 0, true);
                    }
                }
                break;

                case SDL_MOUSEMOTION:
                {
                    const SDL_MouseMotionEvent& motionEvt = evt.motion;

                    int x = motionEvt.x;
                    int y = motionEvt.y;
                    if (__mouseCaptured)
                    {
                        if (x == __mouseCapturePointX && y == __mouseCapturePointY)
                        {
                            // Discard the first MotionNotify following capture since it contains bogus x,y data.
                            break;
                        }

                        // Convert to deltas
                        x -= __mouseCapturePointX;
                        y -= __mouseCapturePointY;

                        // Warp mouse back to center of screen.
                        SDL_WarpMouseInWindow(__window, __mouseCapturePointX, __mouseCapturePointY);
                    }

                    if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_MOVE, x, y, 0))
                    {
                        //if (evt.xmotion.state & Button1Mask)
                        if (evt.button.button == SDL_BUTTON_LEFT)
                        {
                            gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_MOVE, x, y, 0, true);
                        }
                    }
                }
                break;


                case SDL_KEYDOWN:
                {
                    const SDL_KeyboardEvent& keyEvent = evt.key;
                    Keyboard::Key key = translateKey(keyEvent.keysym.scancode);
                    gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_PRESS, key);

                }
                break;

                case SDL_KEYUP:
                {
                    const SDL_KeyboardEvent& keyEvent = evt.key;
                    Keyboard::Key key = translateKey(keyEvent.keysym.scancode);
                    gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_RELEASE, key);
                }
                break;
            }
        }
    }

    SDL_DestroyWindow(__window);
    SDL_Quit();

    return 0;

}

void Platform::swapBuffers()
{
    GP_ERROR("Fix me !");
}

void Platform::signalShutdown()
{
    GP_ERROR("Fix me !");
}

bool Platform::canExit()
{
    GP_ERROR("Fix me !");
    return false;
}

unsigned int Platform::getDisplayWidth()
{
    return __windowSize[0];
}

unsigned int Platform::getDisplayHeight()
{
    return __windowSize[1];
}

double Platform::getAbsoluteTime()
{
    auto now = std::chrono::high_resolution_clock::now();
    typedef std::chrono::duration<double, std::milli> duration;
    duration elapsed = now - __timeStart;
    __timeAbsolute = elapsed.count();
    return __timeAbsolute;
}

void Platform::setAbsoluteTime(double time)
{
    __timeAbsolute = time;
}

bool Platform::isVsync()
{
    return Renderer::getInstance().isVSync();
}

void Platform::setVsync(bool enable)
{
     Renderer::getInstance().setVSync(enable);
}

void Platform::sleep(long ms)
{
    GP_ERROR("Fix me !");
}

void Platform::setMultiSampling(bool enabled)
{
    GP_ERROR("Fix me !");
}

bool Platform::isMultiSampling()
{
    GP_ERROR("Fix me !");
    return false;
}

void Platform::setMultiTouch(bool enabled)
{
    __multiTouch = enabled;
}

bool Platform::isMultiTouch()
{
    return __multiTouch;
}

bool Platform::hasMouse()
{
#ifdef GP_PLATFORM_LINUX
    return true;
#elif GP_PLATFORM_WINDOWS
    return true;
#elif GP_PLATFORM_MACOS
    return true;
#elif GP_PLATFORM_ANDROID
    return false;
#elif GP_PLATFORM_IOS
    return false;
#else
    GP_ERROR("Fix me !");
    return false;
#endif
}

void Platform::setMouseCaptured(bool captured)
{
    if (captured != __mouseCaptured)
    {
        if (captured)
        {
            // Hide the cursor and warp it to the center of the screen
            __mouseCapturePointX = getDisplayWidth() / 2;
            __mouseCapturePointY = getDisplayHeight() / 2;

            setCursorVisible(false);
            SDL_CaptureMouse(SDL_TRUE);
        }
        else
        {
            // Restore cursor
            setCursorVisible(true);
            SDL_CaptureMouse(SDL_FALSE);
        }

        __mouseCaptured = captured;
    }
}

bool Platform::isMouseCaptured()
{
    return __mouseCaptured;
}

void Platform::setCursorVisible(bool visible)
{
    if (visible != __cursorVisible)
    {
        if (visible==false)
        {
            SDL_ShowCursor(false);
        }
        else
        {
            SDL_ShowCursor(true);
        }

        __cursorVisible = visible;
    }
}

bool Platform::isCursorVisible()
{
    return __cursorVisible;
}

bool Platform::hasAccelerometer()
{
#ifdef GP_PLATFORM_LINUX
    return false;
#elif GP_PLATFORM_WINDOWS
    return false;
#elif GP_PLATFORM_MACOS
    return false;
#elif GP_PLATFORM_ANDROID
    return true;
#elif GP_PLATFORM_IOS
    return true;
#else
    GP_ERROR("Fix me !");
    return false;
#endif
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
#if defined(GP_PLATFORM_LINUX) || defined(GP_PLATFORM_WINDOWS) || defined(GP_PLATFORM_MACOS)
    GP_ASSERT(pitch);
    GP_ASSERT(roll);
    *pitch = 0;
    *roll = 0;
#elif GP_PLATFORM_ANDROID
    GP_ERROR("Fix me !");
#elif GP_PLATFORM_IOS
    GP_ERROR("Fix me !");
#else
    GP_ERROR("Fix me !");
#endif
}

void Platform::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    GP_ERROR("Fix me !");
}

void Platform::getArguments(int* argc, char*** argv)
{
    if (argc)
        *argc = __argc;
    if (argv)
        *argv = __argv;
}

void Platform::displayKeyboard(bool display)
{
#if defined(GP_PLATFORM_LINUX) || defined(GP_PLATFORM_WINDOWS) || defined(GP_PLATFORM_MACOS)
    // Do nothing.
#elif GP_PLATFORM_ANDROID
    GP_ERROR("Fix me !");
#elif GP_PLATFORM_IOS
    GP_ERROR("Fix me !");
#else
    GP_ERROR("Fix me !");
#endif
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    GP_ERROR("Fix me !");
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
    GP_ERROR("Fix me !");
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
    GP_ERROR("Fix me !");
}

bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    GP_ERROR("Fix me !");
    return false;
}

bool Platform::launchURL(const char* url)
{
    GP_ERROR("Fix me !");
    return false;
}

std::string Platform::displayFileDialog(size_t mode, const char* title, const char* filterDescription, const char* filterExtensions, const char* initialDirectory)
{
    GP_ERROR("Fix me !");
    return "";
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
    GP_ERROR("Fix me !");
}







}
