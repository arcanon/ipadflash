#!/bin/sh
cd ${TOOLS_SOURCE}

git clone git://git.gnome.org/libxml2 libxml2

cd libxml2

./autogen.sh --prefix=$LIBXML2_PREFIX \
	    --host=arm-apple-darwin9

make && make install
