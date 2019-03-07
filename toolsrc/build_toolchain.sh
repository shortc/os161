#!/bin/sh

$CORES=5

#install binutils

cd binutils-2.24+os161-2.1
find . -name '*.info' | xargs touch
touch intl/plural.c
./configure --nfp --disable-werror --target=mips-harvard-os161 --prefix=$HOME/os161/tools
make -j $CORES
make install
cd ..

cd gcc-4.8.3+os161-2.1
find . -name '*.info' | xargs touch
touch intl/plural.c

