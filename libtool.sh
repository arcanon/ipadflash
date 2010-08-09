#!/bin/sh

cd ${TOOLS_SOURCE}

tar -xzf libtool-2.2.10.tar.gz

cd libtool-2.2.10


./configure --prefix=$LIBTOOL_PREFIX \
	    --host=arm-apple-darwin9

make

#make -j2 && make install




