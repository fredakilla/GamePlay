#!/bin/bash

OUTPWD=../build/res/shaders
PLATFORM=linux

mkdir -p $OUTPWD

echo "build physics"
./shaderc -f  physics.vs -o  $OUTPWD/physics.vert --varyingdef physics.io --type v --platform $PLATFORM
./shaderc -f  physics.fs -o  $OUTPWD/physics.frag --varyingdef physics.io --type f --platform $PLATFORM

echo "build sprite"
./shaderc -f  sprite.vs -o  $OUTPWD/sprite.vert --varyingdef sprite.io --type v --platform $PLATFORM
./shaderc -f  sprite.fs -o  $OUTPWD/sprite.frag --varyingdef sprite.io --type f --platform $PLATFORM

echo "build font"
./shaderc -f  font.vs -o  $OUTPWD/font.vert --varyingdef font.io --type v --platform $PLATFORM
./shaderc -f  font.fs -o  $OUTPWD/font.frag --varyingdef font.io --type f --platform $PLATFORM
./shaderc -f  font_distfield.fs -o  $OUTPWD/font_distfield.frag --varyingdef font.io --type f --platform $PLATFORM

echo "build colored"
./shaderc -f  colored.vs -o  $OUTPWD/colored.vert --varyingdef colored.io --type v --platform $PLATFORM
./shaderc -f  colored.fs -o  $OUTPWD/colored.frag --varyingdef colored.io --type f --platform $PLATFORM
./shaderc -f  colored.fs -o  $OUTPWD/colored_vertex.frag --varyingdef colored.io --type f --platform $PLATFORM --define VERTEX_COLOR

echo "build textured"
./shaderc -f  textured.vs -o  $OUTPWD/textured.vert --varyingdef textured.io --type v --platform $PLATFORM
./shaderc -f  textured.fs -o  $OUTPWD/textured.frag --varyingdef textured.io --type f --platform $PLATFORM

echo "build water2d"
./shaderc -f  water2d.fs -o  $OUTPWD/../common/sprites/water2d.frag --varyingdef sprite.io --type f --platform $PLATFORM

echo "build colored instanced"
./shaderc -f  colored_instanced.vs -o  $OUTPWD/colored_instanced.vert --varyingdef colored_instanced.io --type v --platform $PLATFORM


echo "build light"
./shaderc -f  light.vs -o  $OUTPWD/light.vert --varyingdef light.io --type v --platform $PLATFORM
./shaderc -f  light.fs -o  $OUTPWD/light.frag --varyingdef light.io --type f --platform $PLATFORM

echo "build light"
./shaderc -f  debug.vs -o  $OUTPWD/debug.vert --varyingdef debug.io --type v --platform $PLATFORM
./shaderc -f  debug.fs -o  $OUTPWD/debug.frag --varyingdef debug.io --type f --platform $PLATFORM

echo "build light"
./shaderc -f  debug2.vs -o  $OUTPWD/debug2.vert --varyingdef debug2.io --type v --platform $PLATFORM
./shaderc -f  debug2.fs -o  $OUTPWD/debug2.frag --varyingdef debug2.io --type f --platform $PLATFORM

