#!/bin/sh
cd ${TOOLS_SOURCE}

#wget http://cgit.freedesktop.org/pixman/snapshot/pixman-0.18.2.tar.gz

#tar -xzf pixman-0.18.2.tar.gz

cd pixman-0.18.2

#./autogen.sh

./configure --prefix=$PIXMAN_PREFIX --target=arm-apple-darwin9 --host=arm-apple-darwin9

make && make install
