#--------------------------------------------------------------------
# global var
#--------------------------------------------------------------------

linux: BGFX = /home/fred/Documents/fredakilla/bgfx
linux: BX = /home/fred/Documents/fredakilla/bx

#--------------------------------------------------------------------
# output directory
#--------------------------------------------------------------------

CONFIG(debug,debug|release){
    DESTDIR = $$PWD/../../build
} else {
    DESTDIR = $$PWD/../../build
}

QMAKE_CLEAN += $$DESTDIR/$$TARGET


#--------------------------------------------------------------------
# libraries includes
#--------------------------------------------------------------------

INCLUDEPATH += ../../gameplay/src/ai
INCLUDEPATH += ../../gameplay/src/animation
INCLUDEPATH += ../../gameplay/src/audio
INCLUDEPATH += ../../gameplay/src/core
INCLUDEPATH += ../../gameplay/src/graphics
INCLUDEPATH += ../../gameplay/src/lua
INCLUDEPATH += ../../gameplay/src/math
INCLUDEPATH += ../../gameplay/src/org
INCLUDEPATH += ../../gameplay/src/physics
INCLUDEPATH += ../../gameplay/src/script
INCLUDEPATH += ../../gameplay/src/ui
INCLUDEPATH += ../../gameplay/src/renderer

INCLUDEPATH += $${BGFX}/include
INCLUDEPATH += $${BX}/include
INCLUDEPATH += $${BGFX}/tools/shaderc                      # include shaderc as lib for runtime compile

#--------------------------------------------------------------------
# library depends
#--------------------------------------------------------------------

unix:!macx:PRE_TARGETDEPS += $${DESTDIR}/libgameplay.a

CONFIG(debug,debug|release) {
    message(debug)
    linux:PRE_TARGETDEPS += $${DESTDIR}/libgameplay.a
    #linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libshadercDebug.a
    linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libbgfxDebug.a
    linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libbxDebug.a
    linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libbimgDebug.a

} else {
    message(release)
    linux:PRE_TARGETDEPS += $${DESTDIR}/libgameplay.a
    linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libbgfxRelease.a
    linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libbxRelease.a
    linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libbimgRelease.a
    #linux:PRE_TARGETDEPS += $${BGFX}/.build/linux64_gcc/bin/libshadercRelease.a
}




#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------
QT -= core gui
TARGET = Test
TEMPLATE = app
CONFIG += c++11
CONFIG -= qt

SOURCES += src/TestSample.cpp

HEADERS += src/TestSample.h

INCLUDEPATH += ../../gameplay/src
INCLUDEPATH += ../../external-deps/include
DEFINES += GP_USE_GAMEPAD

linux: DEFINES += __linux__
linux: QMAKE_CXXFLAGS += -lstdc++ -pthread -w
linux: INCLUDEPATH += /usr/include/gtk-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
linux: INCLUDEPATH += /usr/include/atk-1.0
linux: INCLUDEPATH += /usr/include/cairo
linux: INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
linux: INCLUDEPATH += /usr/include/pango-1.0
linux: INCLUDEPATH += /usr/include/gio-unix-2.0
linux: INCLUDEPATH += /usr/include/freetype2
linux: INCLUDEPATH += /usr/include/glib-2.0
linux: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
linux: INCLUDEPATH += /usr/include/pixman-1
linux: INCLUDEPATH += /usr/include/libpng12
linux: INCLUDEPATH += /usr/include/harfbuzz
linux: LIBS += -L$${DESTDIR} -lgameplay
linux: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps
linux: LIBS += -lm -lGL -lrt -ldl -lX11 -lpthread -lgtk-x11-2.0 -lglib-2.0 -lgobject-2.0 -lsndio
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders ../res$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui ../res$$escape_expand(\n\t))
linux: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/../../gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))

CONFIG(debug,debug|release) {
    message(debug)
    #LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lshadercDebug
    LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lbgfxDebug
    LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lbimgDebug


    #LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lshadercDebug
    #LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lfcppDebug
    #LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lglsl-optimizerDebug
    #LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lglslangDebug

    LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lbxDebug

} else {
    message(release)

    LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lbgfxRelease
    LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lbimgRelease
    LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lbxRelease
    #LIBS += -L$${BGFX}/.build/linux64_gcc/bin -lshadercRelease
}




macx: QMAKE_CXXFLAGS += -x c++ -stdlib=libc++ -w -arch x86_64
macx: QMAKE_OBJECTIVE_CFLAGS += -x objective-c++ -stdlib=libc++ -w -arch x86_64
macx: LIBS += -L$$PWD/../../gameplay/Debug/ -lgameplay
macx: LIBS += -L$$PWD/../../external-deps/lib/macosx/x86_64/ -lgameplay-deps
macx: LIBS += -F/System/Library/Frameworks -framework GameKit
macx: LIBS += -F/System/Library/Frameworks -framework IOKit
macx: LIBS += -F/System/Library/Frameworks -framework QuartzCore
macx: LIBS += -F/System/Library/Frameworks -framework OpenAL
macx: LIBS += -F/System/Library/Frameworks -framework OpenGL
macx: LIBS += -F/System/Library/Frameworks -framework Cocoa
macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/shaders ../res$$escape_expand(\n\t))
macx: QMAKE_POST_LINK += $$quote(rsync -rau $$PWD/../../gameplay/res/ui ../res$$escape_expand(\n\t))
macx: QMAKE_POST_LINK += $$quote(cp -rf $$PWD/../../gameplay/res/logo_powered_white.png ../res$$escape_expand(\n\t))
macx 
{
    icon.files = icon.png
    icon.path = Contents/Resources
    QMAKE_BUNDLE_DATA += icon

    gameconfig.files = game.config
    gameconfig.path = Contents/Resources
    QMAKE_BUNDLE_DATA += gameconfig

    res.files = res
    res.path = Contents/Resources
    QMAKE_BUNDLE_DATA += res
}

win32: DEFINES += WIN32 _WINDOWS _UNICODE UNICODE
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../gameplay/Debug/debug/ -lgameplay
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../gameplay/Release/release/ -lgameplay
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Debug/ -lgameplay-deps
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Release/ -lgameplay-deps
win32: LIBS += -lOpenGL32 -lGLU32 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
win32: LIBS += -L$$(DXSDK_DIR)Lib\x64 -lXInput
win32: INCLUDEPATH += $$(DXSDK_DIR)Include
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34189
win32: QMAKE_POST_LINK += $$quote(xcopy ..\../..\gameplay\res\shaders res\shaders\* /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(xcopy ..\../..\gameplay\res\ui res\ui\* /s /y /d$$escape_expand(\n\t))
win32: QMAKE_POST_LINK += $$quote(copy ..\../..\gameplay\res\logo_powered_white.png res$$escape_expand(\n\t))


