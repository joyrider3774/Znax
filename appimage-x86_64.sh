#!/bin/sh

NAME="Znax"
PROJ="znax"
ARCH="x86_64"
DATA="znaxfs"
SDL_LIB_DIR="/home/joyrider3774/dev/x86_64/sdl2/lib"

rm -rf "$PROJ.AppDir"
mkdir -p "$PROJ.AppDir/usr/bin"
mkdir -p "$PROJ.AppDir/usr/lib"
cp "$PROJ" "$PROJ.AppDir/usr/bin"
cp "$DATA/data/$PROJ".png "$PROJ.AppDir"

cp -R "$DATA" "$PROJ.AppDir/usr"
cd "$PROJ.AppDir"
cp "$SDL_LIB_DIR"/* usr/lib
rm usr/lib/*.a
strip usr/bin/* usr/lib/*

echo "[Desktop Entry]
Type=Application
Name=$NAME
Exec=$PROJ
Categories=Game;
Icon=$PROJ" > "$PROJ.desktop"

wget -O AppRun https://github.com/probonopd/AppImageKit/releases/download/12/AppRun-x86_64
chmod +x AppRun

cd ..
wget -N https://github.com/probonopd/AppImageKit/releases/download/12/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage
./appimagetool-x86_64.AppImage "$PROJ.AppDir" "$PROJ-$ARCH.AppImage"

