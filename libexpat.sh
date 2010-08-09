#!/bin/sh
cd ${TOOLS_SOURCE}

#cvs -d:pserver:anonymous@expat.cvs.sourceforge.net:/cvsroot/expat login
#cvs -z3 -d:pserver:anonymous@expat.cvs.sourceforge.net:/cvsroot/expat co expat

cd expat

#./buildconf.sh

./configure --prefix=$LIBEXPAT_PREFIX \
	    --host=arm-apple-darwin9 \
	    --target=arm-apple-darwin9

make
make install

