#!/bin/sh

echo "Building..."

make clean
make CPPFLAGS="-I${HOME}/dev/x86_64/sdl2/include -I${HOME}/dev/x86_64/sdl2/include/SDL2" LDFLAGS="-L${HOME}/dev/x86_64/sdl2/lib -L/usr/lib"

echo "Making AppImage..."

./appimage-x86_64.sh 
