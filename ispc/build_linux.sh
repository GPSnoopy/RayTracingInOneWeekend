#!/bin/sh
set -e

mkdir --parents build/linux
cd build/linux
cmake -D ISPC_EXECUTABLE=/home/gpsnoopy/Development/ispc-v1.15.0-linux/bin/ispc -D X86_64_CPU=1 ../..
make -j
