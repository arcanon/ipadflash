#!/bin/sh
cd ${TOOLS_SOURCE}

#wget http://cairographics.org/snapshots/cairo-1.9.12.tar.gz

#tar -xzf cairo-1.9.12.tar.gz

cd cairo-1.9.12

export pixman_CFLAGS="-I$PIXMAN_PREFIX/include/pixman-1"
export pixman_LIBS="-L$PIXMAN_PREFIX/lib -lpixman-1"

export FONTCONFIG_CFLAGS="-I$FONTCONFIG_PREFIX/include"
export FONTCONFIG_LIBS="-L$FONTCONFIG_PREFIX/lib -lfontconfig"
export FREETYPE_CFLAGS="-I$FREETYPE_PREFIX/include/freetype2 -I$FREETYPE_PREFIX/include"
export FREETYPE_LIBS="-L$FREETYPE_PREFIX/lib -lfreetype"
export png_CFLAGS="-I$LIBPNG_PREFIX/include"
export png_LIBS="-L$LIBPNG_PREFIX/lib -lpng"

./configure --prefix=$CAIRO_PREFIX \
	    --host=arm-apple-darwin9 \
	    --target=arm-apple-darwin9 \
	    --disable-xlib \
	    --disable-dependency-tracking \
	    --disable-quartz \
	    --enable-xlib-xrender=no

make && make install
