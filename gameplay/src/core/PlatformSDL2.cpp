#ifndef GP_NO_PLATFORM
#ifdef __linux__

#include "../core/Base.h"
#include "../core/Platform.h"
#include "../core/FileSystem.h"
#include "../core/Game.h"
#include "../ui/Form.h"
#include "../script/ScriptController.h"

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <fstream>


#include <SDL2/SDL_config.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/platform.h>
#include "../renderer/BGFX/BGFXRenderer.h"

#define TOUCH_COUNT_MAX     4
#define MAX_GAMEPADS 4

using namespace std;

int __argc = 0;
char** __argv = 0;

enum GamepadAxisInfoFlags
{
    GP_AXIS_SKIP = 0x1,
    GP_AXIS_IS_DPAD = 0x2,
    GP_AXIS_IS_NEG = 0x4,
    GP_AXIS_IS_XAXIS = 0x8,
    GP_AXIS_IS_TRIGGER = 0x10
};

enum GamepadAxisInfoNormalizeFunction
{
    NEG_TO_POS,
    ZERO_TO_POS
};

struct GamepadJoystickAxisInfo
{
    int axisIndex;
    unsigned int joystickIndex;
    unsigned long flags;
    int mappedPosArg;
    int mappedNegArg;
    float deadZone;
    GamepadAxisInfoNormalizeFunction mapFunc;
};

struct GamepadInfoEntry
{
    unsigned int vendorId;
    unsigned int productId;
    const char* productName;
    unsigned int numberOfJS;
    unsigned int numberOfAxes;
    unsigned int numberOfButtons;
    unsigned int numberOfTriggers;

    GamepadJoystickAxisInfo* axes;
    long* buttons;
};

struct ConnectedGamepadDevInfo
{
    dev_t deviceId;
    gameplay::GamepadHandle fd;
    const GamepadInfoEntry& gamepadInfo;
};

struct timespec __timespec;
static double __timeStart;
static double __timeAbsolute;
//static bool __vsync = WINDOW_VSYNC;
static bool __mouseCaptured = false;
static float __mouseCapturePointX = 0;
static float __mouseCapturePointY = 0;
static bool __multiSampling = false;
static bool __cursorVisible = true;
//static Display* __display;
//static Window __window;
static int __windowSize[2];
//static GLXContext __context;
//static Atom __atomWmDeleteWindow;
static list<ConnectedGamepadDevInfo> __connectedGamepads;


static SDL_Window * __window;
//static SDL_GLContext __context = 0;






// Gets the gameplay::Keyboard::Key enumeration constant that corresponds to the given X11 key symbol.
static gameplay::Keyboard::Key getKey(KeySym sym)
{
#if X11
    switch (sym)
    {
        case XK_Sys_Req:
            return gameplay::Keyboard::KEY_SYSREQ;
        case XK_Break:
            return gameplay::Keyboard::KEY_BREAK;
        case XK_Menu :
            return gameplay::Keyboard::KEY_MENU;
        case XK_KP_Enter:
            return gameplay::Keyboard::KEY_KP_ENTER;
        case XK_Pause:
            return gameplay::Keyboard::KEY_PAUSE;
        case XK_Scroll_Lock:
            return gameplay::Keyboard::KEY_SCROLL_LOCK;
        case XK_Print:
            return gameplay::Keyboard::KEY_PRINT;
        case XK_Escape:
            return gameplay::Keyboard::KEY_ESCAPE;
        case XK_BackSpace:
            return gameplay::Keyboard::KEY_BACKSPACE;
        case XK_Tab:
            return gameplay::Keyboard::KEY_TAB;
        case XK_Return:
            return gameplay::Keyboard::KEY_RETURN;
        case XK_Caps_Lock:
            return gameplay::Keyboard::KEY_CAPS_LOCK;
        case XK_Shift_L:
        case XK_Shift_R:
            return gameplay::Keyboard::KEY_SHIFT;
        case XK_Control_L:
        case XK_Control_R:
            return gameplay::Keyboard::KEY_CTRL;
        case XK_Alt_L:
        case XK_Alt_R:
            return gameplay::Keyboard::KEY_ALT;
        case XK_Hyper_L:
        case XK_Hyper_R:
            return gameplay::Keyboard::KEY_HYPER;
        case XK_Insert:
            return gameplay::Keyboard::KEY_INSERT;
        case XK_Home:
            return gameplay::Keyboard::KEY_HOME;
        case XK_Page_Up:
            return gameplay::Keyboard::KEY_PG_UP;
        case XK_Delete:
            return gameplay::Keyboard::KEY_DELETE;
        case XK_End:
            return gameplay::Keyboard::KEY_END;
        case XK_Page_Down:
            return gameplay::Keyboard::KEY_PG_DOWN;
        case XK_Left:
            return gameplay::Keyboard::KEY_LEFT_ARROW;
        case XK_Right:
            return gameplay::Keyboard::KEY_RIGHT_ARROW;
        case XK_Up:
            return gameplay::Keyboard::KEY_UP_ARROW;
        case XK_Down:
            return gameplay::Keyboard::KEY_DOWN_ARROW;
        case XK_Num_Lock:
            return gameplay::Keyboard::KEY_NUM_LOCK;
        case XK_KP_Add:
            return gameplay::Keyboard::KEY_KP_PLUS;
        case XK_KP_Subtract:
            return gameplay::Keyboard::KEY_KP_MINUS;
        case XK_KP_Multiply:
            return gameplay::Keyboard::KEY_KP_MULTIPLY;
        case XK_KP_Divide:
            return gameplay::Keyboard::KEY_KP_DIVIDE;
        case XK_KP_Home:
            return gameplay::Keyboard::KEY_KP_HOME;
        case XK_KP_Up:
            return gameplay::Keyboard::KEY_KP_UP;
        case XK_KP_Page_Up:
            return gameplay::Keyboard::KEY_KP_PG_UP;
        case XK_KP_Left:
            return gameplay::Keyboard::KEY_KP_LEFT;
        case XK_KP_5:
            return gameplay::Keyboard::KEY_KP_FIVE;
        case XK_KP_Right:
            return gameplay::Keyboard::KEY_KP_RIGHT;
        case XK_KP_End:
            return gameplay::Keyboard::KEY_KP_END;
        case XK_KP_Down:
            return gameplay::Keyboard::KEY_KP_DOWN;
        case XK_KP_Page_Down:
            return gameplay::Keyboard::KEY_KP_PG_DOWN;
        case XK_KP_Insert:
            return gameplay::Keyboard::KEY_KP_INSERT;
        case XK_KP_Delete:
            return gameplay::Keyboard::KEY_KP_DELETE;
        case XK_F1:
            return gameplay::Keyboard::KEY_F1;
        case XK_F2:
            return gameplay::Keyboard::KEY_F2;
        case XK_F3:
            return gameplay::Keyboard::KEY_F3;
        case XK_F4:
            return gameplay::Keyboard::KEY_F4;
        case XK_F5:
            return gameplay::Keyboard::KEY_F5;
        case XK_F6:
            return gameplay::Keyboard::KEY_F6;
        case XK_F7:
            return gameplay::Keyboard::KEY_F7;
        case XK_F8:
            return gameplay::Keyboard::KEY_F8;
        case XK_F9:
            return gameplay::Keyboard::KEY_F9;
        case XK_F10:
            return gameplay::Keyboard::KEY_F10;
        case XK_F11:
            return gameplay::Keyboard::KEY_F11;
        case XK_F12:
            return gameplay::Keyboard::KEY_F12;
        case XK_KP_Space:
        case XK_space:
            return gameplay::Keyboard::KEY_SPACE;
        case XK_parenright:
            return gameplay::Keyboard::KEY_RIGHT_PARENTHESIS;
        case XK_0:
            return gameplay::Keyboard::KEY_ZERO;
        case XK_exclam:
            return gameplay::Keyboard::KEY_EXCLAM;
        case XK_1:
            return gameplay::Keyboard::KEY_ONE;
        case XK_at:
            return gameplay::Keyboard::KEY_AT;
        case XK_2:
            return gameplay::Keyboard::KEY_TWO;
        case XK_numbersign:
            return gameplay::Keyboard::KEY_NUMBER;
        case XK_3:
            return gameplay::Keyboard::KEY_THREE;
        case XK_dollar:
            return gameplay::Keyboard::KEY_DOLLAR;
        case XK_4:
            return gameplay::Keyboard::KEY_FOUR;
        case XK_percent:
        case XK_asciicircum :
            return gameplay::Keyboard::KEY_CIRCUMFLEX;
            return gameplay::Keyboard::KEY_PERCENT;
        case XK_5:
            return gameplay::Keyboard::KEY_FIVE;
        case XK_6:
            return gameplay::Keyboard::KEY_SIX;
        case XK_ampersand:
            return gameplay::Keyboard::KEY_AMPERSAND;
        case XK_7:
            return gameplay::Keyboard::KEY_SEVEN;
        case XK_asterisk:
            return gameplay::Keyboard::KEY_ASTERISK;
        case XK_8:
            return gameplay::Keyboard::KEY_EIGHT;
        case XK_parenleft:
            return gameplay::Keyboard::KEY_LEFT_PARENTHESIS;
        case XK_9:
            return gameplay::Keyboard::KEY_NINE;
        case XK_equal:
            return gameplay::Keyboard::KEY_EQUAL;
        case XK_plus:
            return gameplay::Keyboard::KEY_PLUS;
        case XK_less:
            return gameplay::Keyboard::KEY_LESS_THAN;
        case XK_comma:
            return gameplay::Keyboard::KEY_COMMA;
        case XK_underscore:
            return gameplay::Keyboard::KEY_UNDERSCORE;
        case XK_minus:
            return gameplay::Keyboard::KEY_MINUS;
        case XK_greater:
            return gameplay::Keyboard::KEY_GREATER_THAN;
        case XK_period:
            return gameplay::Keyboard::KEY_PERIOD;
        case XK_colon:
            return gameplay::Keyboard::KEY_COLON;
        case XK_semicolon:
            return gameplay::Keyboard::KEY_SEMICOLON;
        case XK_question:
            return gameplay::Keyboard::KEY_QUESTION;
        case XK_slash:
            return gameplay::Keyboard::KEY_SLASH;
        case XK_grave:
            return gameplay::Keyboard::KEY_GRAVE;
        case XK_asciitilde:
            return gameplay::Keyboard::KEY_TILDE;
        case XK_braceleft:
            return gameplay::Keyboard::KEY_LEFT_BRACE;
        case XK_bracketleft:
            return gameplay::Keyboard::KEY_LEFT_BRACKET;
        case XK_bar:
            return gameplay::Keyboard::KEY_BAR;
        case XK_backslash:
            return gameplay::Keyboard::KEY_BACK_SLASH;
        case XK_braceright:
            return gameplay::Keyboard::KEY_RIGHT_BRACE;
        case XK_bracketright:
            return gameplay::Keyboard::KEY_RIGHT_BRACKET;
        case XK_quotedbl:
            return gameplay::Keyboard::KEY_QUOTE;
        case XK_apostrophe:
            return gameplay::Keyboard::KEY_APOSTROPHE;
        case XK_EuroSign:
            return gameplay::Keyboard::KEY_EURO;
        case XK_sterling:
            return gameplay::Keyboard::KEY_POUND;
        case XK_yen:
            return gameplay::Keyboard::KEY_YEN;
        case XK_periodcentered:
            return gameplay::Keyboard::KEY_MIDDLE_DOT;
        case XK_A:
            return gameplay::Keyboard::KEY_CAPITAL_A;
        case XK_a:
            return gameplay::Keyboard::KEY_A;
        case XK_B:
            return gameplay::Keyboard::KEY_CAPITAL_B;
        case XK_b:
            return gameplay::Keyboard::KEY_B;
        case XK_C:
            return gameplay::Keyboard::KEY_CAPITAL_C;
        case XK_c:
            return gameplay::Keyboard::KEY_C;
        case XK_D:
            return gameplay::Keyboard::KEY_CAPITAL_D;
        case XK_d:
            return gameplay::Keyboard::KEY_D;
        case XK_E:
            return gameplay::Keyboard::KEY_CAPITAL_E;
        case XK_e:
            return gameplay::Keyboard::KEY_E;
        case XK_F:
            return gameplay::Keyboard::KEY_CAPITAL_F;
        case XK_f:
            return gameplay::Keyboard::KEY_F;
        case XK_G:
            return gameplay::Keyboard::KEY_CAPITAL_G;
        case XK_g:
            return gameplay::Keyboard::KEY_G;
        case XK_H:
            return gameplay::Keyboard::KEY_CAPITAL_H;
        case XK_h:
            return gameplay::Keyboard::KEY_H;
        case XK_I:
            return gameplay::Keyboard::KEY_CAPITAL_I;
        case XK_i:
            return gameplay::Keyboard::KEY_I;
        case XK_J:
            return gameplay::Keyboard::KEY_CAPITAL_J;
        case XK_j:
            return gameplay::Keyboard::KEY_J;
        case XK_K:
            return gameplay::Keyboard::KEY_CAPITAL_K;
        case XK_k:
            return gameplay::Keyboard::KEY_K;
        case XK_L:
            return gameplay::Keyboard::KEY_CAPITAL_L;
        case XK_l:
            return gameplay::Keyboard::KEY_L;
        case XK_M:
            return gameplay::Keyboard::KEY_CAPITAL_M;
        case XK_m:
            return gameplay::Keyboard::KEY_M;
        case XK_N:
            return gameplay::Keyboard::KEY_CAPITAL_N;
        case XK_n:
            return gameplay::Keyboard::KEY_N;
        case XK_O:
            return gameplay::Keyboard::KEY_CAPITAL_O;
        case XK_o:
            return gameplay::Keyboard::KEY_O;
        case XK_P:
            return gameplay::Keyboard::KEY_CAPITAL_P;
        case XK_p:
            return gameplay::Keyboard::KEY_P;
        case XK_Q:
            return gameplay::Keyboard::KEY_CAPITAL_Q;
        case XK_q:
            return gameplay::Keyboard::KEY_Q;
        case XK_R:
            return gameplay::Keyboard::KEY_CAPITAL_R;
        case XK_r:
            return gameplay::Keyboard::KEY_R;
        case XK_S:
            return gameplay::Keyboard::KEY_CAPITAL_S;
        case XK_s:
            return gameplay::Keyboard::KEY_S;
        case XK_T:
            return gameplay::Keyboard::KEY_CAPITAL_T;
        case XK_t:
            return gameplay::Keyboard::KEY_T;
        case XK_U:
            return gameplay::Keyboard::KEY_CAPITAL_U;
        case XK_u:
            return gameplay::Keyboard::KEY_U;
        case XK_V:
            return gameplay::Keyboard::KEY_CAPITAL_V;
        case XK_v:
            return gameplay::Keyboard::KEY_V;
        case XK_W:
            return gameplay::Keyboard::KEY_CAPITAL_W;
        case XK_w:
            return gameplay::Keyboard::KEY_W;
        case XK_X:
            return gameplay::Keyboard::KEY_CAPITAL_X;
        case XK_x:
            return gameplay::Keyboard::KEY_X;
        case XK_Y:
            return gameplay::Keyboard::KEY_CAPITAL_Y;
        case XK_y:
            return gameplay::Keyboard::KEY_Y;
        case XK_Z:
            return gameplay::Keyboard::KEY_CAPITAL_Z;
        case XK_z:
            return gameplay::Keyboard::KEY_Z;
        default:
            return gameplay::Keyboard::KEY_NONE;
    }

#endif
}

/**
 * Returns the unicode value for the given keycode or zero if the key is not a valid printable character.
 */
static int getUnicode(gameplay::Keyboard::Key key)
{
    switch (key)
    {
        case gameplay::Keyboard::KEY_BACKSPACE:
            return 0x0008;
        case gameplay::Keyboard::KEY_TAB:
            return 0x0009;
        case gameplay::Keyboard::KEY_RETURN:
        case gameplay::Keyboard::KEY_KP_ENTER:
            return 0x000A;
        case gameplay::Keyboard::KEY_ESCAPE:
            return 0x001B;
        case gameplay::Keyboard::KEY_SPACE:
        case gameplay::Keyboard::KEY_EXCLAM:
        case gameplay::Keyboard::KEY_QUOTE:
        case gameplay::Keyboard::KEY_NUMBER:
        case gameplay::Keyboard::KEY_DOLLAR:
        case gameplay::Keyboard::KEY_PERCENT:
        case gameplay::Keyboard::KEY_CIRCUMFLEX:
        case gameplay::Keyboard::KEY_AMPERSAND:
        case gameplay::Keyboard::KEY_APOSTROPHE:
        case gameplay::Keyboard::KEY_LEFT_PARENTHESIS:
        case gameplay::Keyboard::KEY_RIGHT_PARENTHESIS:
        case gameplay::Keyboard::KEY_ASTERISK:
        case gameplay::Keyboard::KEY_PLUS:
        case gameplay::Keyboard::KEY_COMMA:
        case gameplay::Keyboard::KEY_MINUS:
        case gameplay::Keyboard::KEY_PERIOD:
        case gameplay::Keyboard::KEY_SLASH:
        case gameplay::Keyboard::KEY_ZERO:
        case gameplay::Keyboard::KEY_ONE:
        case gameplay::Keyboard::KEY_TWO:
        case gameplay::Keyboard::KEY_THREE:
        case gameplay::Keyboard::KEY_FOUR:
        case gameplay::Keyboard::KEY_FIVE:
        case gameplay::Keyboard::KEY_SIX:
        case gameplay::Keyboard::KEY_SEVEN:
        case gameplay::Keyboard::KEY_EIGHT:
        case gameplay::Keyboard::KEY_NINE:
        case gameplay::Keyboard::KEY_COLON:
        case gameplay::Keyboard::KEY_SEMICOLON:
        case gameplay::Keyboard::KEY_LESS_THAN:
        case gameplay::Keyboard::KEY_EQUAL:
        case gameplay::Keyboard::KEY_GREATER_THAN:
        case gameplay::Keyboard::KEY_QUESTION:
        case gameplay::Keyboard::KEY_AT:
        case gameplay::Keyboard::KEY_CAPITAL_A:
        case gameplay::Keyboard::KEY_CAPITAL_B:
        case gameplay::Keyboard::KEY_CAPITAL_C:
        case gameplay::Keyboard::KEY_CAPITAL_D:
        case gameplay::Keyboard::KEY_CAPITAL_E:
        case gameplay::Keyboard::KEY_CAPITAL_F:
        case gameplay::Keyboard::KEY_CAPITAL_G:
        case gameplay::Keyboard::KEY_CAPITAL_H:
        case gameplay::Keyboard::KEY_CAPITAL_I:
        case gameplay::Keyboard::KEY_CAPITAL_J:
        case gameplay::Keyboard::KEY_CAPITAL_K:
        case gameplay::Keyboard::KEY_CAPITAL_L:
        case gameplay::Keyboard::KEY_CAPITAL_M:
        case gameplay::Keyboard::KEY_CAPITAL_N:
        case gameplay::Keyboard::KEY_CAPITAL_O:
        case gameplay::Keyboard::KEY_CAPITAL_P:
        case gameplay::Keyboard::KEY_CAPITAL_Q:
        case gameplay::Keyboard::KEY_CAPITAL_R:
        case gameplay::Keyboard::KEY_CAPITAL_S:
        case gameplay::Keyboard::KEY_CAPITAL_T:
        case gameplay::Keyboard::KEY_CAPITAL_U:
        case gameplay::Keyboard::KEY_CAPITAL_V:
        case gameplay::Keyboard::KEY_CAPITAL_W:
        case gameplay::Keyboard::KEY_CAPITAL_X:
        case gameplay::Keyboard::KEY_CAPITAL_Y:
        case gameplay::Keyboard::KEY_CAPITAL_Z:
        case gameplay::Keyboard::KEY_LEFT_BRACKET:
        case gameplay::Keyboard::KEY_BACK_SLASH:
        case gameplay::Keyboard::KEY_RIGHT_BRACKET:
        case gameplay::Keyboard::KEY_UNDERSCORE:
        case gameplay::Keyboard::KEY_GRAVE:
        case gameplay::Keyboard::KEY_A:
        case gameplay::Keyboard::KEY_B:
        case gameplay::Keyboard::KEY_C:
        case gameplay::Keyboard::KEY_D:
        case gameplay::Keyboard::KEY_E:
        case gameplay::Keyboard::KEY_F:
        case gameplay::Keyboard::KEY_G:
        case gameplay::Keyboard::KEY_H:
        case gameplay::Keyboard::KEY_I:
        case gameplay::Keyboard::KEY_J:
        case gameplay::Keyboard::KEY_K:
        case gameplay::Keyboard::KEY_L:
        case gameplay::Keyboard::KEY_M:
        case gameplay::Keyboard::KEY_N:
        case gameplay::Keyboard::KEY_O:
        case gameplay::Keyboard::KEY_P:
        case gameplay::Keyboard::KEY_Q:
        case gameplay::Keyboard::KEY_R:
        case gameplay::Keyboard::KEY_S:
        case gameplay::Keyboard::KEY_T:
        case gameplay::Keyboard::KEY_U:
        case gameplay::Keyboard::KEY_V:
        case gameplay::Keyboard::KEY_W:
        case gameplay::Keyboard::KEY_X:
        case gameplay::Keyboard::KEY_Y:
        case gameplay::Keyboard::KEY_Z:
        case gameplay::Keyboard::KEY_LEFT_BRACE:
        case gameplay::Keyboard::KEY_BAR:
        case gameplay::Keyboard::KEY_RIGHT_BRACE:
        case gameplay::Keyboard::KEY_TILDE:
            return key;
        default:
            return 0;
    }
}

// Included here to avoid the naming conflict between KEY_* defined in input.h and the ones defined in gameplay/Keyboard.h 
#include <linux/joystick.h> 

namespace gameplay
{

Renderer* Renderer::_instance = nullptr;

extern void print(const char* format, ...)
{
    GP_ASSERT(format);
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

extern int strcmpnocase(const char* s1, const char* s2)
{
    return strcasecmp(s1, s2);
}

void updateWindowSize()
{
    //GP_ASSERT(__display);
    GP_ASSERT(__window);

    int width;
    int height;
    SDL_GetWindowSize(__window, &width, &height);

    Renderer::getInstance().updateWindowSize(width, height);

    __windowSize[0] = width;
    __windowSize[1] = height;
}

Platform::Platform(Game* game) : _game(game)
{
}

Platform::~Platform()
{
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
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.ndt          = wmi.info.x11.display;
    pd.nwh          = (void*)(uintptr_t)wmi.info.x11.window;
#	elif BX_PLATFORM_OSX
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.win.window;
#	elif BX_PLATFORM_STEAMLINK
    pd.ndt          = wmi.info.vivante.display;
    pd.nwh          = wmi.info.vivante.window;
#	endif // BX_PLATFORM_
    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    return true;
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
    __window = SDL_CreateWindow("SDL2",
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
    game->setViewport(Rectangle(0,0,__width,__height), 0);

    updateWindowSize();

    return platform;
}

void cleanupX11()
{
    //if (__display)
    {
        Renderer::getInstance().destroy();

        bgfx::shutdown();

        //@@glXMakeCurrent(__display, None, NULL);

        //@@if (__context)
        //@@    glXDestroyContext(__display, __context);
       // if (__window)
       //     XDestroyWindow(__display, __window);

        if(__window)
            SDL_DestroyWindow(__window);

       // XCloseDisplay(__display);
    }
}

double timespec2millis(struct timespec *a)
{
    GP_ASSERT(a);
    return (1000.0 * a->tv_sec) + (0.000001 * a->tv_nsec);
}

// Will need to be dynamic, also should be handled in Gamepad class
static const GamepadInfoEntry gamepadLookupTable[] = 
{
    {0x0,0x0,"Microsoft Xbox 360 Controller",2,6,20,2, 
                                            (GamepadJoystickAxisInfo[]) {
                                                                    {0,0,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                    {1,0,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                    {2,1,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                    {3,1,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                    {4,2,GP_AXIS_IS_TRIGGER,0,0,2240,ZERO_TO_POS},
                                                                    {5,2,GP_AXIS_IS_TRIGGER,1,0,2240,ZERO_TO_POS},
                                                                    {-1,0,0,0,0,0,NEG_TO_POS}
                                                                },
                                            (long[]) {
                                                                        -1,    
                                                                        -1,  
                                                                        -1,  
                                                                        -1, 
                                                                        -1, 
                                                                        Gamepad::BUTTON_UP,
                                                                        Gamepad::BUTTON_DOWN,
                                                                        Gamepad::BUTTON_LEFT,
                                                                        Gamepad::BUTTON_RIGHT,
                                                                        Gamepad::BUTTON_MENU2,
                                                                        Gamepad::BUTTON_MENU1,
                                                                        Gamepad::BUTTON_L3,
                                                                        Gamepad::BUTTON_R3,
                                                                        Gamepad::BUTTON_L1,
                                                                        Gamepad::BUTTON_R1,
                                                                        Gamepad::BUTTON_MENU3,
                                                                        Gamepad::BUTTON_A,
                                                                        Gamepad::BUTTON_B,
                                                                        Gamepad::BUTTON_X,
                                                                        Gamepad::BUTTON_Y
                                                                        }
    },
    {0x54c,0x268,"Sony PlayStation 3 Controller",2,27,19,2, 
                                            (GamepadJoystickAxisInfo[]) {
                                                                    {0,0,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                    {1,0,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                    {2,1,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                    {3,1,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                    {12,1,GP_AXIS_IS_TRIGGER,0,0,2240,ZERO_TO_POS},
                                                                    {13,2,GP_AXIS_IS_TRIGGER,1,0,2240,ZERO_TO_POS},
                                                                    {-1,0,0,0,0,0,NEG_TO_POS}
                                                                },
                                            (long[]) {
                                                                        Gamepad::BUTTON_MENU1,    
                                                                        Gamepad::BUTTON_L3,  
                                                                        Gamepad::BUTTON_R3,  
                                                                        Gamepad::BUTTON_MENU2, 
                                                                        Gamepad::BUTTON_UP, 
                                                                        Gamepad::BUTTON_RIGHT, 
                                                                        Gamepad::BUTTON_DOWN,    
                                                                        Gamepad::BUTTON_LEFT,   
                                                                        Gamepad::BUTTON_L2,  //Use Trigger Instead of BUTTON_L2? or both should be called
                                                                        Gamepad::BUTTON_R2,  //Use Trigger Instead of BUTTON_R2? or both should be called                                                                        
                                                                        Gamepad::BUTTON_L1,
                                                                        Gamepad::BUTTON_R1,
                                                                        Gamepad::BUTTON_Y,    
                                                                        Gamepad::BUTTON_B,  
                                                                        Gamepad::BUTTON_A,  
                                                                        Gamepad::BUTTON_X, 
                                                                        Gamepad::BUTTON_MENU3, 
                                                                        -1,
                                                                        -1
                                                                        }
    },
    {0x79,0x6,"Generic USB Controller",2,7,12,0, 
                                            (GamepadJoystickAxisInfo[]) {
                                                                    {0,1, GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                    {1,1,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                    {2,0,GP_AXIS_SKIP,0,0,2240,NEG_TO_POS},
                                                                    {3,0,GP_AXIS_IS_XAXIS,0,0,2240,NEG_TO_POS},
                                                                    {4,0,GP_AXIS_IS_NEG,0,0,2240,NEG_TO_POS},
                                                                    {5,2,GP_AXIS_IS_DPAD, Gamepad::BUTTON_RIGHT, Gamepad::BUTTON_LEFT,2240,NEG_TO_POS},
                                                                    {6,2,GP_AXIS_IS_DPAD, Gamepad::BUTTON_DOWN, Gamepad::BUTTON_UP,2240,NEG_TO_POS},
                                                                    {-1,0,0,0,0,0,NEG_TO_POS}
                                                                },
                                            (long[]) {
                                                                        Gamepad::BUTTON_Y,    
                                                                        Gamepad::BUTTON_B,  
                                                                        Gamepad::BUTTON_A,  
                                                                        Gamepad::BUTTON_X, 
                                                                        Gamepad::BUTTON_L1, 
                                                                        Gamepad::BUTTON_R1, 
                                                                        Gamepad::BUTTON_L2,    
                                                                        Gamepad::BUTTON_R2,   
                                                                        Gamepad::BUTTON_MENU1,   
                                                                        Gamepad::BUTTON_MENU2,   
                                                                        Gamepad::BUTTON_L3,
                                                                        Gamepad::BUTTON_R3,
                                                                        }
    }
};

bool isGamepadDevRegistered(dev_t devId)
{
    for (list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end(); ++it)
    {
        if (devId == (*it).deviceId) 
            return true;
    }
    return false;
}

void closeGamepad(const ConnectedGamepadDevInfo& gamepadDevInfo)
{
    ::close(gamepadDevInfo.fd);
}

void unregisterGamepad(GamepadHandle handle)
{
    for (list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end(); ++it)
    {
        if (handle == (*it).fd)
        {
            closeGamepad(*it);
            __connectedGamepads.erase(it);
            return;
        }
    }
}

void closeAllGamepads()
{
    for (list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end(); ++it)
    {
        closeGamepad(*it);
        __connectedGamepads.erase(it);
    }
}

const GamepadInfoEntry& getGamepadMappedInfo(unsigned int vendorId, unsigned int productId, unsigned int numberOfAxes, unsigned int numberOfButtons)
{
    for (int i = 0; i<sizeof(gamepadLookupTable)/sizeof(GamepadInfoEntry); i++)
    {
        const GamepadInfoEntry& curEntry = gamepadLookupTable[i];
        if (curEntry.vendorId == vendorId && curEntry.productId == productId)
        {
            return curEntry;
        }
    }

    for (int i=0;i<sizeof(gamepadLookupTable)/sizeof(GamepadInfoEntry);i++)
    {
        const GamepadInfoEntry& curEntry = gamepadLookupTable[i];
        if (curEntry.vendorId == 0 && curEntry.productId == 0 && curEntry.numberOfAxes == numberOfAxes && curEntry.numberOfButtons == numberOfButtons)
        {
            return curEntry;
        }
    }

    return gamepadLookupTable[0];
}

const GamepadInfoEntry& getGamepadMappedInfo(const GamepadHandle handle)
{
    GP_ASSERT(handle >= 0);

    for (list<ConnectedGamepadDevInfo>::iterator it = __connectedGamepads.begin(); it != __connectedGamepads.end();++it)
    {
        if (handle == (*it).fd)
        {
            return it->gamepadInfo;
        }
    }
    GP_WARN("Gamepad not connected but yet trying to get its data. Falling back to generic one.");
    return gamepadLookupTable[0];
}

const GamepadJoystickAxisInfo* tryGetGamepadMappedAxisInfo(const GamepadInfoEntry& gpinfo, unsigned int axisNumber)
{
    if (axisNumber >= 0 && axisNumber < gpinfo.numberOfAxes)
    {
        int i = 0;
        while (true)
        {
            const GamepadJoystickAxisInfo* curAxisInfo = &gpinfo.axes[i++];
            if (curAxisInfo->axisIndex == axisNumber)
                return curAxisInfo;
            else if (curAxisInfo->axisIndex < 0)
                return NULL;
        }
    }
    return NULL;
}

bool tryGetGamepadMappedButton(const GamepadInfoEntry& gpinfo, unsigned long btnNumber, long& outMap)
{
    if (btnNumber >= 0 && btnNumber < gpinfo.numberOfButtons )
    {
        if (gpinfo.buttons[btnNumber] >= 0)
        {
            outMap = gpinfo.buttons[btnNumber];
            return true;
        }
        else
        {
            return false;
        }
    }
    GP_WARN("Unmapped gamepad button: %u.",btnNumber);
    return false;
}

unsigned int readIntegerGamepadIdPropery(const char* sysFSIdPath, const char* propertyName)
{
    unsigned int ret = 0;
    try 
    {
        ifstream propStream;
        propStream.open((string(sysFSIdPath) + propertyName).c_str(),ifstream::in);
        propStream >> std::hex >> ret;
        propStream.close();
    } 
    catch (exception e) 
    {
        GP_WARN("Could not read propery from SysFS for Gamepad: %s", propertyName);
    }
    return ret;
}

bool isBlackListed(unsigned int vendorId, unsigned int productId)
{
    switch (vendorId)
    {
        case 0x0e0f: //virtual machine devices
            if (productId == 0x0003) // Virtual Mouse
                return true;
    }
    return false;
}

void handleConnectedGamepad(dev_t devId, const char* devPath, const char* sysFSIdPath)
{
    GP_ASSERT(devPath);

    unsigned int vendorId =readIntegerGamepadIdPropery(sysFSIdPath,"vendor");
    unsigned int productId =readIntegerGamepadIdPropery(sysFSIdPath,"product");

    if (isBlackListed(vendorId, productId))
        return;

    GamepadHandle handle = ::open(devPath,O_RDONLY | O_NONBLOCK);
    if(handle < 0)
    {
        GP_WARN("Could not open Gamepad device.");
        return;
    }

    if (!(fcntl(handle, F_GETFL) != -1 || errno != EBADF))
        return;

    char axesNum, btnsNum, name[256];
    ioctl(handle, JSIOCGNAME(256), name);
    ioctl (handle, JSIOCGAXES, &axesNum);
    ioctl (handle, JSIOCGBUTTONS, &btnsNum);

    const GamepadInfoEntry& gpInfo = getGamepadMappedInfo(vendorId, productId, (unsigned int)axesNum, (unsigned int)btnsNum);
    unsigned int numJS = gpInfo.numberOfJS;
    unsigned int numTR = gpInfo.numberOfTriggers;

    // Ignore accelerometer devices that register themselves as joysticks. Ensure they have at least 2 buttons.
    if (btnsNum < 2)
        return;

    Platform::gamepadEventConnectedInternal(handle, btnsNum, numJS, numTR, name);
    ConnectedGamepadDevInfo info = {devId,handle,gpInfo}; 
    __connectedGamepads.push_back(info);
}

static float normalizeJoystickAxis(int axisValue, int deadZone, bool zeroToOne)
{
    int absAxisValue = 0;
    if (zeroToOne)
        absAxisValue = (axisValue + 32767) / 2.0;
    else
        absAxisValue = abs(axisValue);

    if (absAxisValue < deadZone)
    {
        return 0.0f;
    }
    else
    {
        int maxVal = 0;
        int value = 0;
        if(!zeroToOne)
        {
            value = axisValue;
            if (value < 0)
            {
                value = -1;
                maxVal = 32768;
            }
            else if (value > 0)
            {
                value = 1;
                maxVal = 32767;
            }
            else
            {
                return 0.0f;
            }
        }
        else
        {
            value = 1;
            maxVal = 32767;
        }

        float ret = value * (absAxisValue - deadZone) / (float)(maxVal - deadZone);
        return ret;
    }
}

void enumGamepads()
{
    const int maxDevs = 16;
    const char* devPathFormat = "/dev/input/js%u";
    const char* sysfsPathFormat = "/sys/class/input/js%u/device/id/";
    char curDevPath[20];

    for(int i=0;i<maxDevs;i++)
    {
        sprintf(curDevPath,devPathFormat,i);
        struct stat gpstat;
        if(::stat(curDevPath,&gpstat) == 0)
        {
            dev_t devid = gpstat.st_rdev;
            if(!isGamepadDevRegistered(devid))
            {
                char cursysFSPath[35];
                sprintf(cursysFSPath,sysfsPathFormat,i);
                handleConnectedGamepad(devid,curDevPath,cursysFSPath);
            }
        }
    }
}

void gamepadHandlingLoop()
{
    enumGamepads();
}

int Platform::enterMessagePump()
{
    GP_ASSERT(_game);

    //updateWindowSize();

    static bool shiftDown = false;
    static bool capsOn = false;


    // Get the initial time.
    clock_gettime(CLOCK_REALTIME, &__timespec);
    __timeStart = timespec2millis(&__timespec);
    __timeAbsolute = 0L;

    // Run the game.
    _game->run();


    Renderer * renderer = &Renderer::getInstance();


    bool loop = true;
    while (loop)
    {
        if (_game)
        {
            renderer->beginFrame(); //bgfx::touch(0);
            _game->frame();
            renderer->endFrame(); //bgfx::frame();

        }



        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                loop = false;

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    loop = false;
                    break;
                case SDLK_F2:
                    Renderer::getInstance().toggleDebugStats();
                    break;
                case SDLK_F7:
                    Renderer::getInstance().toggleVSync();
                    break;

                default:
                    break;
                }


                KeySym sym = event.key.keysym.sym;

                Keyboard::Key key = getKey(sym);
                gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_PRESS, key);

                if (key == Keyboard::KEY_CAPS_LOCK)
                    capsOn = !capsOn;
                if (key == Keyboard::KEY_SHIFT)
                    shiftDown = true;

                if (int character = getUnicode(key))
                    gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_CHAR, character);


            }

            if (event.type == SDL_KEYUP)
            {
                KeySym sym = event.key.keysym.sym;
                Keyboard::Key key = getKey(sym);
                gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_RELEASE, key);

                if (key == Keyboard::KEY_SHIFT)
                    shiftDown = false;
            }


            if (event.type == SDL_MOUSEWHEEL)
            {
                int wheelDelta;
                if(event.wheel.y == 1) // scroll up
                {
                    wheelDelta = 1;
                }
                else if(event.wheel.y == -1) // scroll down
                {
                    wheelDelta = -1;
                }
                else
                    wheelDelta = 0;

                gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_WHEEL, 0, 0, wheelDelta);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                gameplay::Mouse::MouseEvent mouseEvt;

                switch (event.button.button)
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

                if (!gameplay::Platform::mouseEventInternal(mouseEvt, event.button.x, event.button.y, 0))
                {
                    gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_PRESS, event.button.x, event.button.y, 0, true);
                }


            }

            if (event.type == SDL_MOUSEBUTTONUP)
            {
                gameplay::Mouse::MouseEvent mouseEvt;

                switch (event.button.button)
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

                if (!gameplay::Platform::mouseEventInternal(mouseEvt, event.button.x, event.button.y, 0))
                {
                    gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_RELEASE, event.button.x, event.button.y, 0, true);
                }
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                int x = event.motion.x;
                int y = event.motion.y;
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
                    //XWarpPointer(__display, None, __window, 0, 0, 0, 0, __mouseCapturePointX, __mouseCapturePointY);
                    SDL_WarpMouseInWindow(__window, __mouseCapturePointX, __mouseCapturePointY);
                }

                if (!gameplay::Platform::mouseEventInternal(gameplay::Mouse::MOUSE_MOVE, x, y, 0))
                {
                    //if (evt.xmotion.state & Button1Mask)
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        gameplay::Platform::touchEventInternal(gameplay::Touch::TOUCH_MOVE, x, y, 0, true);
                    }
                }
            }


        }
    }

    cleanupX11();

    return 0;

}

void Platform::signalShutdown()
{
}

bool Platform::canExit()
{
    return true;
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

    clock_gettime(CLOCK_REALTIME, &__timespec);
    double now = timespec2millis(&__timespec);
    __timeAbsolute = now - __timeStart;

    return __timeAbsolute;
}

void Platform::setAbsoluteTime(double time)
{
    __timeAbsolute = time;
}

bool Platform::isVsync()
{
    return Renderer::getInstance().isVSync();
    //@@return __vsync;
}

void Platform::setVsync(bool enable)
{
    //@@__vsync = enable;

    Renderer::getInstance().setVSync(enable);

    /* BGFXRenderer* renderer = (BGFXRenderer*)Renderer::getInstance();
     renderer.*/

    //@@if (glXSwapIntervalEXT)
    //@@    glXSwapIntervalEXT(__display, __window, __vsync ? 1 : 0);
    //@@else if(glXSwapIntervalMESA)
    //@@    glXSwapIntervalMESA(__vsync ? 1 : 0);
}

void Platform::swapBuffers()
{
    //@@glXSwapBuffers(__display, __window);
    //bgfx::frame();
}

void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}

void Platform::setMultiSampling(bool enabled)
{
    if (enabled == __multiSampling)
    {
        return;
    }
        
        // TODO
        __multiSampling = enabled;
}
    
    bool Platform::isMultiSampling()
    {
        return __multiSampling;
    }

void Platform::setMultiTouch(bool enabled)
{
    // not supported
}

bool Platform::isMultiTouch()
{
    false;
}

bool Platform::hasAccelerometer()
{
    return false;
}

void Platform::getAccelerometerValues(float* pitch, float* roll)
{
    GP_ASSERT(pitch);
    GP_ASSERT(roll);

    *pitch = 0;
    *roll = 0;
}

void Platform::getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ)
{
    if (accelX)
    {
        *accelX = 0;
    }

    if (accelY)
    {
        *accelY = 0;
    }

    if (accelZ)
    {
        *accelZ = 0;
    }

    if (gyroX)
    {
        *gyroX = 0;
    }

    if (gyroY)
    {
        *gyroY = 0;
    }

    if (gyroZ)
    {
        *gyroZ = 0;
    }
}

void Platform::getArguments(int* argc, char*** argv)
{
    if (argc)
        *argc = __argc;
    if (argv)
        *argv = __argv;
}

bool Platform::hasMouse()
{
    return true;
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
            //XWarpPointer(__display, None, __window, 0, 0, 0, 0, __mouseCapturePointX, __mouseCapturePointY);
            SDL_CaptureMouse(SDL_TRUE);
        }
        else
        {
            // Restore cursor
            //XWarpPointer(__display, None, __window, 0, 0, 0, 0, __mouseCapturePointX, __mouseCapturePointY);
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
            /*Cursor invisibleCursor;
            Pixmap bitmapNoData;
            XColor black;
            static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
            black.red = black.green = black.blue = 0;
            bitmapNoData = XCreateBitmapFromData(__display, __window, noData, 8, 8);
            invisibleCursor = XCreatePixmapCursor(__display, bitmapNoData, bitmapNoData, &black, &black, 0, 0);

            XDefineCursor(__display, __window, invisibleCursor);
            XFreeCursor(__display, invisibleCursor);
            XFreePixmap(__display, bitmapNoData);*/

            SDL_ShowCursor(false);
        }
        else
        {
            //XUndefineCursor(__display, __window);
            SDL_ShowCursor(true);
        }
        //XFlush(__display);
        __cursorVisible = visible;
    }
}

bool Platform::isCursorVisible()
{
    return __cursorVisible;
}

void Platform::displayKeyboard(bool display)
{
    // not supported
}

void Platform::shutdownInternal()
{
    closeAllGamepads();
    Game::getInstance()->shutdown();
}

bool Platform::isGestureSupported(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::registerGesture(Gesture::GestureEvent evt)
{
}

void Platform::unregisterGesture(Gesture::GestureEvent evt)
{
}

bool Platform::isGestureRegistered(Gesture::GestureEvent evt)
{
    return false;
}

void Platform::pollGamepadState(Gamepad* gamepad)
{
    GP_ASSERT(gamepad);

    struct js_event jevent;
    const GamepadInfoEntry& gpInfo = getGamepadMappedInfo(gamepad->_handle);

    while (read(gamepad->_handle, &jevent, sizeof(struct js_event)) > 0)
    {
        switch (jevent.type)
        {
            case JS_EVENT_BUTTON:
            case JS_EVENT_BUTTON | JS_EVENT_INIT:
                {
                    long curMappingIndex = -1;
                    if(tryGetGamepadMappedButton(gpInfo, jevent.number, curMappingIndex))
                    {
                        unsigned int buttons = 0;
                        if (jevent.value)
                            buttons |= (1 << curMappingIndex);
                        else
                            buttons &= ~(1 << curMappingIndex);
                        gamepad->setButtons(buttons);
                    }
                    break;
                }
            case JS_EVENT_AXIS:
            case JS_EVENT_AXIS | JS_EVENT_INIT:
                {
                    if(jevent.number < gpInfo.numberOfAxes)
                    {
                        const GamepadJoystickAxisInfo* jsInfo = tryGetGamepadMappedAxisInfo(gpInfo,jevent.number);
                        if(jsInfo)
                        {
                            float val = normalizeJoystickAxis(jevent.value,jsInfo->deadZone,jsInfo->mapFunc == ZERO_TO_POS);
                            if(!(jsInfo->flags & GP_AXIS_SKIP))
                            {
                                if((jsInfo->flags & GP_AXIS_IS_NEG))
                                    val = -1.0f * val;

                                bool not_js_axis = false;
                                if((jsInfo->flags & GP_AXIS_IS_DPAD))
                                {
                                    unsigned int buttons = 0;
                                    if(jevent.value != 0)
                                        buttons |= (1 << (jevent.value > 0 ? jsInfo->mappedPosArg : jsInfo->mappedNegArg));
                                    else
                                    {
                                        buttons &= ~(1 << jsInfo->mappedPosArg);
                                        buttons &= ~(1 << jsInfo->mappedNegArg);
                                    }
                                    gamepad->setButtons(buttons);
                                    not_js_axis = true;
                                }
                                if((jsInfo->flags & GP_AXIS_IS_TRIGGER))
                                {
                                    gamepad->setTriggerValue(jsInfo->mappedPosArg, val);
                                    not_js_axis = true;
                                }

                                if(!not_js_axis)
                                {
                                    Vector2 jsVals;
                                    gamepad->getJoystickValues(jsInfo->joystickIndex,&jsVals);
                                    if(jsInfo->flags & GP_AXIS_IS_XAXIS)
                                        jsVals.x = val;
                                    else
                                        jsVals.y = val;
                                    gamepad->setJoystickValue(jsInfo->joystickIndex,jsVals.x,jsVals.y);
                                }
                            }
                        }
                    }
                }
                break;

            default: 
                GP_WARN("unhandled gamepad event: %x\n", jevent.type);
        }
    }
    if(errno == ENODEV)
    {
        unregisterGamepad(gamepad->_handle);
        gamepadEventDisconnectedInternal(gamepad->_handle);
    }
}

bool Platform::launchURL(const char* url)
{
    if (url == NULL || *url == '\0')
        return false;

    int len = strlen(url);

    char* cmd = new char[11 + len];
    sprintf(cmd, "xdg-open %s", url);
    int r = system(cmd);
    SAFE_DELETE_ARRAY(cmd);

    return (r == 0);
}

std::string Platform::displayFileDialog(size_t mode, const char* title, const char* filterDescription, const char* filterExtensions, const char* initialDirectory)
{
#if X11
    std::string filename = "";

    if (!gtk_init_check(NULL,NULL))
        return "";

    // Create the dialog in one of two modes, SAVE or OPEN
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new(title, NULL,
                                         mode == FileSystem::SAVE ? GTK_FILE_CHOOSER_ACTION_SAVE : GTK_FILE_CHOOSER_ACTION_OPEN,
                                         _("_Cancel"), GTK_RESPONSE_CANCEL,
                                         mode == FileSystem::SAVE ? _("_Save") : _("_Open"),
                                         GTK_RESPONSE_ACCEPT, NULL);

    // Filter on extensions
    GtkFileFilter* filter = gtk_file_filter_new();
    std::istringstream f(filterExtensions);
    std::string s;
    std::string extStr;
    while (std::getline(f, s, ';'))
    {
        extStr = "*.";
        extStr += s;
        gtk_file_filter_add_pattern(filter, extStr.c_str());
    }
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Set initial directory
    std::string initialDirectoryStr;
    if (initialDirectory == NULL)
    {
        char* currentDir = g_get_current_dir();
        initialDirectoryStr = currentDir;
        g_free(currentDir);
    }
    else
    {
        initialDirectoryStr = initialDirectory;
    }
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), initialDirectoryStr.c_str());

    if (mode == FileSystem::SAVE)
    {
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "");
    }

    // Show the dialog
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char* szFilename;
        szFilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        filename = szFilename;
        g_free(szFilename);
    }
    gtk_widget_destroy(dialog);

    // Since we are not using gtk_main(), this will let the dialog close
    while (gtk_events_pending()) 
        gtk_main_iteration();

    return filename;
#endif
}

}

#endif
#endif
