#!/bin/sh

cd ${TOOLS_SOURCE}

git clone git://git.sv.nongnu.org/freetype/freetype2.git freetype2

cd freetype2

chmod +x autogen.sh
./autogen.sh

./configure --prefix=$FREETYPE_PREFIX --target=arm-apple-darwin9 --without-zlib --host=arm-apple-darwin9

make
make install
