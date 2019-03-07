#!/bin/sh

CORES=$(getconf _NPROCESSORS_ONLN)

#install binutils

cd binutils-2.24+os161-2.1
find . -name '*.info' | xargs touch
touch intl/plural.c
./configure --nfp --disable-werror --target=mips-harvard-os161 --prefix=$HOME/os161/tools
make -j $CORES
make install
cd ..

#install gcc

INCP=$CPLUS_INCLUDE_PATH
unset CPLUS_INCLUDE_PATH

cd gcc-4.8.3+os161-2.1
find . -name '*.info' | xargs touch
touch intl/plural.c
cd ..

cd buildgcc
../gcc-4.8.3+os161-2.1/configure \
  --enable-languages=c,lto \
  --nfp --disable-shared --disable-threads \
  --disable-libmudflap --disable-libssp \
  --disable-libstdcxx --disable-nls \
  --target=mips-harvard-os161 \
  --prefix=$HOME/os161/tools
gmake -j $CORES
gmake install
cd ..

#install gdb

cd gdb-7.8+os161-2.1
find . -name '*.info' | xargs touch
touch intl/plural.c
./configure --target=mips-harvard-os161 --prefix=$HOME/os161/tools
make -j $CORES
make install
cd ..

export CPLUS_INCLUDE_PATH=$INCP

#install system 161

cd sys161-2.0.8
./configure --prefix=$HOME/os161/tools mipseb
make -j $CORES
make install
cd ..

cd bmake
./configure --prefix=$HOME/os161/tools --with-default-sys-path=$HOME/os161/tools/share/mk
sh ./make-bootstrap.sh
mkdir -p $HOME/os161/tools/bin
mkdir -p $HOME/os161/tools/share/man/man1
mkdir -p $HOME/os161/tools/share/mk
cp bmake $HOME/os161/tools/bin/
cp bmake.1 $HOME/os161/tools/share/man/man1/
sh mk/install-mk $HOME/os161/tools/share/mk
cd ..

pushd $HOME/os161/tools/bin
sh -c 'for i in mips-*; do ln -s $i os161-`echo $i | cut -d- -f4-`; done'
popd
