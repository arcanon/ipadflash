#!/bin/sh
cd ${TOOLS_SOURCE}

#git clone git://git.ffmpeg.org/ffmpeg/
cd ffmpeg
#git clone git://git.ffmpeg.org/libswscale/

#cd ..

./configure --cc=$CC \
	    --ld=$CC \
	    --ar=$AR \
	    --cross-prefix=$BINDIR/arm-apple-darwin9- \
	    --enable-cross-compile \
	    --target-os=darwin \
	    --arch=arm-apple-darwin9 \
	    --prefix=$FFMPEG_PREFIX \
	    --enable-pic \
	    --disable-mmx \
	    --disable-mmx2 \
	    --disable-asm \
        --enable-shared


#make && make install
