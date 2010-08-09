#!/bin/sh
SIMULATOR="/Users/pcontezini/devel/iphoneFlash/simulator"
SIMULATOR_SDK="/Developer/Platforms/iPhoneSimulator.platform/"
SIMULATOR_SDK_DEVROOT="/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator3.2.sdk"


export CC="/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/i686-apple-darwin10-gcc-4.2.1"
export CFLAGS="-I~/devel/iphoneFlash/simulator/include"

export CC="/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/i686-apple-darwin10-gcc-4.2.1"
export CXX="/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/i686-apple-darwin10-gcc-4.2.1"

export CFLAGS="-isysroot $SIMULATOR_SDK_DEVROOT -I$SIMULATOR/include -mmacosx-version-min=10.5"
export CPPFLAGS="-isysroot $SIMULATOR_SDK_DEVROOT -I$SIMULATOR/include -mmacosx-version-min=10.5"

export LDFLAGS="-L$SIMULATOR/lib"

./configure --prefix=$SIMULATOR --target=i686-apple-darwin10
