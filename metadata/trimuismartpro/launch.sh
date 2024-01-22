#!/bin/sh
echo $0 $*
PROGDIR=/mnt/SDCARD/Apps/znax

cd $PROGDIR
HOME=$PROGDIR/ ./znax -r
