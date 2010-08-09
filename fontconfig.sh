#!/bin/sh
cd ${TOOLS_SOURCE}

git clone git://anongit.freedesktop.org/git/fontconfig fontconfig 

cd fontconfig

#./autogen.sh

export LDFLAGS="-L/usr/lib -lstdc++ "

./configure --prefix=$FONTCONFIG_PREFIX \
	    --build=x86 \
	    --host=arm-apple-darwin9 \
	    --target=arm-apple-darwin9 \
	    --with-expat-includes=$LIBEXPAT_PREFIX/include \
	    --with-expat-lib=$LIBEXPAT_PREFIX/lib \
	    --with-freetype-config=$FREETYPE_PREFIX/bin/freetype-config \
	    --with-arch=arm-apple-darwin9

make
#make install

