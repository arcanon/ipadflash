#!/bin/sh
cd ${TOOLS_SOURCE}

#wget "http://downloads.sourceforge.net/project/giflib/giflib%204.x/giflib-4.1.6/giflib-4.1.6.tar.bz2?use_mirror=sunet&ts=1279386772"

#tar -xjf giflib-4.1.6.tar.bz2

cd giflib-4.1.6

./configure --prefix=$GIFLIB_PREFIX --host=arm-apple-darwin9

make && make install
