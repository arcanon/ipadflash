#!/bin/sh
cd ${TOOLS_SOURCE}

#Notes:
# bypass darwin substitution in configure file
#PREFERED_PLATFORM=X11
#case "$host" in
#  *pdarwin* )
# bypass SDL detection and enable by default
#ENABLE_SDL_TRUE='#'
#  ENABLE_SDL_FALSE=

#cvs -d:pserver:anonymous@vector-agg.cvs.sourceforge.net:/cvsroot/vector-agg login 
 
#cvs -z3 -d:pserver:anonymous@vector-agg.cvs.sourceforge.net:/cvsroot/vector-agg co agg-2.5

cd agg-2.5

#chmod +x autogen.sh

#autoreconf

#./autogen.sh

export FREETYPE_CFLAGS="-I$FREETYPE_PREFIX/include/freetype2 -I$FREETYPE_PREFIX/include"
export FREETYPE_LIBS="-L$FREETYPE_PREFIX/lib -lfreetype"

./configure --prefix=$AGG_PREFIX \
	    --host=arm-apple-darwin9 \
	    --target=arm-apple-darwin9 \
	    --with-x=no \
	    --enable-examples=no
   
make && make install
