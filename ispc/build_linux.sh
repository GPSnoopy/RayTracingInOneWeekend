#!/bin/sh
set -e

mkdir --parents build/linux
cd build/linux
cmake -D ISPC_EXECUTABLE=/home/gpsnoopy/Development/ispc-v1.16.1-linux/bin/ispc ../..
make -j
