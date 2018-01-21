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

echo "build colored instanced"
./shadercDebug -f  colored_instanced.vs -o  $OUTPWD/colored_instanced.vert --varyingdef colored_instanced.io --type v --platform $PLATFORM


echo "build light"
./shadercDebug -f  light.vs -o  $OUTPWD/light.vert --varyingdef light.io --type v --platform $PLATFORM
./shadercDebug -f  light.fs -o  $OUTPWD/light.frag --varyingdef light.io --type f --platform $PLATFORM