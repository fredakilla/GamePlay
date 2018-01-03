#!/bin/bash

OUTPWD=..
PLATFORM=linux
SHADERC=shadercDebug

mkdir -p $OUTPWD

echo "compiling PhysicsDebug"
./shadercDebug -f  PhysicsDebug.vs -o  $OUTPWD/PhysicsDebug_VS.bin --varyingdef PhysicsDebug.io --type v --platform $PLATFORM
./shadercDebug -f  PhysicsDebug.fs -o  $OUTPWD/PhysicsDebug_FS.bin --varyingdef PhysicsDebug.io --type f --platform $PLATFORM

echo "compiling Sprite"
./shadercDebug -f  Sprite.vs -o  $OUTPWD/Sprite_VS.bin --varyingdef Sprite.io --type v --platform $PLATFORM
./shadercDebug -f  Sprite.fs -o  $OUTPWD/Sprite_FS.bin --varyingdef Sprite.io --type f --platform $PLATFORM

echo "compiling Colored"
./shadercDebug -f  Colored.vs -o  $OUTPWD/Colored_VS.bin --varyingdef Colored.io --type v --platform $PLATFORM
./shadercDebug -f  Colored.fs -o  $OUTPWD/Colored_FS.bin --varyingdef Colored.io --type f --platform $PLATFORM

echo "compiling Textured"
./shadercDebug -f  Textured.vs -o  $OUTPWD/Textured_VS.bin --varyingdef Textured.io --type v --platform $PLATFORM
./shadercDebug -f  Textured.fs -o  $OUTPWD/Textured_FS.bin --varyingdef Textured.io --type f --platform $PLATFORM

echo "compiling Font"
./shadercDebug -f  Font.vs -o  $OUTPWD/Font_VS.bin --varyingdef Font.io --type v --platform $PLATFORM
./shadercDebug -f  Font.fs -o  $OUTPWD/Font_FS.bin --varyingdef Font.io --type f --platform $PLATFORM



echo "compiling Textured"
./shadercDebug -f  Textured.vs -o  $OUTPWD/Textured_TEXTURE_REPEAT_VS.bin --varyingdef Textured.io --type v --platform $PLATFORM --define TEXTURE_REPEAT
./shadercDebug -f  Textured.fs -o  $OUTPWD/Textured_DISCARD_ALPHA_FS.bin --varyingdef Textured.io --type f --platform $PLATFORM --define TEXTURE_DISCARD_ALPHA
