#!/bin/sh
set -e

mkdir --parents build/linux
cd build/linux
cmake -D ISPC_EXECUTABLE=/home/gpsnoopy/development/ispc-v1.14.1-linux/bin/ispc ../..
make -j
