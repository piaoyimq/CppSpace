#!/bin/bash
set -e

cmake .
make 
cp lib*.a $WS_ROOT/staging/Linux_x86/usr/local/lib
cp -r include/* $WS_ROOT/staging/Linux_x86/usr/local/include
git clean -fd
