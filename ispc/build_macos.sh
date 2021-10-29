#!/bin/sh
set -e

mkdir -p build/macos
cd build/macos
cmake -D ISPC_EXECUTABLE=/home/gpsnoopy/Development/ispc-v1.16.1-linux/bin/ispc ../..
make -j
