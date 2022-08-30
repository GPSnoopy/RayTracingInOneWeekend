#!/bin/sh
set -e

brew install ispc
mkdir -p build/macos
cd build/macos
cmake ../..
make -j
