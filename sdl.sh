#!/bin/sh
cd ${TOOLS_SOURCE}

hg clone http://hg.libsdl.org/SDL SDL

#wget http://www.libsdl.org/release/SDL-1.2.14.tar.gz

#tar -xzf SDL-1.2.14.tar.gz

cd SDL

#./autogen.sh

./configure --prefix=$SDL_PREFIX \
	    --host=arm-apple-darwin9 \
	    --target=arm-apple-darwin9 \
	    --enable-video-x11=no
#        --disable-shared \

#make clean
#make && make install
