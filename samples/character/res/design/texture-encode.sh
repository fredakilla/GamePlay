#!/bin/sh

DIR_OUT=encoded
mkdir -p $DIR_OUT

EXT=png
for i in *.$EXT; do
    base=`basename $i .$EXT`
    echo $i
    ./texturecDebug -f $i -o $DIR_OUT/$base.dds -m
done

echo tilesn.png && ./texturecDebug -f tilesn.png -o $DIR_OUT/tilesn.dds -m -n 
echo woodn.png && ./texturecDebug -f woodn.png -o $DIR_OUT/woodn.dds -m -n
