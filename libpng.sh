#!/bin/sh

cd ${TOOLS_SOURCE}

#wget ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.4.3.tar.gz

#tar -xzf libpng-1.4.3.tar.gz

cd libpng-1.4.3

./configure --prefix=$LIBPNG_PREFIX --host=arm-apple-darwin9

make
make install
