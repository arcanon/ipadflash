#!/bin/sh
cd ${TOOLS_SOURCE}

git clone git://github.com/bagder/curl.git curl 

cd curl

./buildconf

./configure --prefix=$CURL_PREFIX \
	    --host=arm-apple-darwin9 \
	    --target=arm-apple-darwin9 \
	    --without-libidn \
	    --without-ssl \
	    --without-zlib

make
make install
