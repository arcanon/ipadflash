#!/bin/sh
#export CPPFLAGS="-I/usr/include/libxml2"

# edit gnashconig.h -> //#define HAVE_LIBDLLOADER 1
#NO_LIBTOOLIZE=1

./configure --with-PACKAGE=$SIMULATOR \
	    --host=arm-apple-darwin9 \
        --enable-shared \
	    --prefix=$TOOLS_PRE/gnash \
	    --enable-ltdl-install=yes \
	    --with-sdl-incl=$TOOLS_PRE/SDL/include/SDL \
	    --with-sdl-lib=$TOOLS_PRE/SDL/lib \
	    --with-gif-incl=$TOOLS_PRE/giflib/include \
	    --with-gif-lib=$TOOLS_PRE/giflib/lib \
	    --enable-renderer=AGG \
	    --enable-cairo=no \
	    --enable-gui=sdl \
	    --enable-media=ffmpeg \
	    --disable-nls \
	    --disable-sound \
	    --with-boost-incl=$TOOLS_PRE/boost/include \
	    --with-boost-lib=$TOOLS_PRE/boost/lib \
	    --with-ffmpeg-incl=$TOOLS_PRE/ffmpeg/include \
	    --with-ffmpeg-lib=$TOOLS_PRE/ffmpeg/lib \
	    --with-png-incl=$TOOLS_PRE/libpng/include \
	    --with-png-lib=$TOOLS_PRE/libpng/lib \
	    --with-jpeg-incl=$TOOLS_PRE/jpeglib/include \
	    --with-jpeg-lib=$TOOLS_PRE/jpeglib/lib \
	    --with-z-incl=$SIMULATOR/usr/include \
	    --with-z-lib=$SIMULATOR/usr/lib \
	    --with-freetype-incl=$TOOLS_PRE/freetype2/include/freetype2 \
	    --with-freetype-lib=$TOOLS_PRE/freetype2/lib \
	    --with-fontconfig-incl=$TOOLS_PRE/fontconfig/include \
	    --with-fontconfig-lib=$TOOLS_PRE/fontconfig/lib  \
	    --with-agg-incl=$TOOLS_PRE/agg/include/agg2 \
	    --with-agg-lib=$TOOLS_PRE/agg/lib \
	    --with-curl-incl=$TOOLS_PRE/curl/include \
	    --with-curl-lib=$TOOLS_PRE/curl/lib \
	    --with-expat-incl=$TOOLS_PRE/libexpat/include \
	    --with-expat-lib=$TOOLS_PRE/libexpat/lib \
	    --with-libxml-incl=$TOOLS_PRE/libxml2/include/libxml2 \
	    --with-libxml-lib=$TOOLS_PRE/libxml2/lib
#	    --with-cairo-incl=$TOOLS_PRE/cairo/include \
#	    --with-cairo-lib=$TOOLS_PRE/cairo/lib \
#	    --with-opengl-includes=$SIMULATOR_SDK_DEVROOT/System/Library/Frameworks/OpenGLES.framework/Headers/ES1
#	    --with-opengl-lib=$SIMULATOR_SDK_DEVROOT/System/Library/Frameworks/OpenGLES.framework/
#	    --with-gstreamer-incl=/opt/local/include/gstreamer-0.10 \
	    #--with-z-incl=$SIMULATOR_SDK_DEVROOT/usr/include \
	    #--with-z-lib=$SIMULATOR_SDK_DEVROOT/usr/lib \
#	    --with-expat-incl=$SIMULATOR_SDK_DEVROOT/usr/include \
#	    --with-expat-lib=$SIMULATOR_SDK_DEVROOT/usr/lib \
# --enable-allstatic

#make -j2

