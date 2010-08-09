#!/bin/sh
cd ${TOOLS_SOURCE}

#wget http://www.ijg.org/files/jpegsrc.v8b.tar.gz

#tar -xzf jpegsrc.v8b.tar.gz

cd jpeg-8b

./configure --prefix=$JPEGLIB_PREFIX --host=arm-apple-darwin9

make
make install
