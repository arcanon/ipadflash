#!/bin/sh
# check out rev 64002
echo $TOOLS_SOURCE
cd $TOOLS_SOURCE

#svn checkout http://svn.boost.org/svn/boost/branches/release -r 64002 boost


#mkdir -p $TOOLS_BUILD/boost
cd boost

bjam --prefix=$BOOST_PREFIX \
     --layout=versioned \
     toolset=darwin \
     architecture=arm \
     target-os=iphone\
     macosx-version=iphone-3.2 \
     define=_LITTLE_ENDIAN \
     link=static \
     install

#in load
#rule iphone-3.1.3 unknown in module darwin.

#./bjam --prefix=${HOME}/dev/boost/iphoneSimulator toolset=darwin architecture=x86 target-os=iphone macosx-version=iphonesim-3.2 link=static install
