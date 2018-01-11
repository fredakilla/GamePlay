#!/bin/bash

OUTPWD=..
PLATFORM=linux
SHADERC=.shadercDebug

mkdir -p $OUTPWD

echo "build physics"
./shadercDebug -f  physics.vs -o  $OUTPWD/physics.vert --varyingdef physics.io --type v --platform $PLATFORM
./shadercDebug -f  physics.fs -o  $OUTPWD/physics.frag --varyingdef physics.io --type f --platform $PLATFORM

echo "build sprite"
./shadercDebug -f  sprite.vs -o  $OUTPWD/sprite.vert --varyingdef sprite.io --type v --platform $PLATFORM
./shadercDebug -f  sprite.fs -o  $OUTPWD/sprite.frag --varyingdef sprite.io --type f --platform $PLATFORM

echo "build font"
./shadercDebug -f  font.vs -o  $OUTPWD/font.vert --varyingdef font.io --type v --platform $PLATFORM
./shadercDebug -f  font.fs -o  $OUTPWD/font.frag --varyingdef font.io --type f --platform $PLATFORM
./shadercDebug -f  font_distfield.fs -o  $OUTPWD/font_distfield.frag --varyingdef font.io --type f --platform $PLATFORM

echo "build colored"
./shadercDebug -f  colored.vs -o  $OUTPWD/colored.vert --varyingdef colored.io --type v --platform $PLATFORM
./shadercDebug -f  colored.fs -o  $OUTPWD/colored.frag --varyingdef colored.io --type f --platform $PLATFORM
./shadercDebug -f  colored.fs -o  $OUTPWD/colored_vertex.frag --varyingdef colored.io --type f --platform $PLATFORM --define VERTEX_COLOR

echo "build textured"
./shadercDebug -f  textured.vs -o  $OUTPWD/textured.vert --varyingdef textured.io --type v --platform $PLATFORM
./shadercDebug -f  textured.fs -o  $OUTPWD/textured.frag --varyingdef textured.io --type f --platform $PLATFORM

echo "build water2d"
./shadercDebug -f  water2d.fs -o  $OUTPWD/../common/sprites/water2d.frag --varyingdef sprite.io --type f --platform $PLATFORM

#echo "compiling Sprite"
#./shadercDebug -f  Sprite.vs -o  $OUTPWD/Sprite_VS.bin --varyingdef Sprite.io --type v --platform $PLATFORM
#./shadercDebug -f  Sprite.fs -o  $OUTPWD/Sprite_FS.bin --varyingdef Sprite.io --type f --platform $PLATFORM
#
#e#cho "compiling Colored"
#./shadercDebug -f  Colored.vs -o  $OUTPWD/Colored_VS.bin --varyingdef Colored.io --type v --platform $PLATFORM
#./shadercDebug -f  Colored.fs -o  $OUTPWD/Colored_FS.bin --varyingdef Colored.io --type f --platform $PLATFORM
#./shadercDebug -f  Colored.fs -o  $OUTPWD/Colored_VERTEX_COLOR_FS.bin --varyingdef Colored.io --type f --platform $PLATFORM --define VERTEX_COLOR
#
#echo "compiling Textured"
#./shadercDebug -f  Textured.vs -o  $OUTPWD/Textured_VS.bin --varyingdef Textured.io --type v --platform $PLATFORM
#./shadercDebug -f  Textured.fs -o  $OUTPWD/Textured_FS.bin --varyingdef Textured.io --type f --platform $PLATFORM
#
#echo "compiling Font"
#./shadercDebug -f  Font.vs -o  $OUTPWD/Font_VS.bin --varyingdef Font.io --type v --platform $PLATFORM
#./shadercDebug -f  Font.fs -o  $OUTPWD/Font_FS.bin --varyingdef Font.io --type f --platform $PLATFORM
#
#
#
#echo "compiling Textured"
#./shadercDebug -f  Textured.vs -o  $OUTPWD/Textured_TEXTURE_REPEAT_VS.bin --varyingdef Textured.io --type v --platform $PLATFORM --define TEXTURE_REPEAT
#./shadercDebug -f  Textured.fs -o  $OUTPWD/Textured_DISCARD_ALPHA_FS.bin --varyingdef Textured.io --type f --platform $PLATFORM --define TEXTURE_DISCARD_ALPHA
#
#
#echo "compiling PostProcess"
#./shadercDebug -f  PostProcess.vs -o  $OUTPWD/PostProcess_VS.bin --varyingdef PostProcess.io --type v --platform $PLATFORM
#./shadercDebug -f  PostProcess.fs -o  $OUTPWD/PostProcess_FS.bin --varyingdef PostProcess.io --type f --platform $PLATFORM
