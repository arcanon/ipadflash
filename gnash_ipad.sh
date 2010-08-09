export SIMULATOR="/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys"

export GNASH=/home/ash/ipad-dev/iphonedevonlinux/apps/gnash
export TOOLS_SOURCE="${GNASH}/src"
export TOOLS_PRE="${GNASH}/pre"
export TOOLS_BUILD="${GNASH}/build/boost"

export BINDIR="/home/ash/ipad-dev/iphonedevonlinux/toolchain/pre/bin"
export CC="$BINDIR/arm-apple-darwin9-gcc"
export CXX="$BINDIR/arm-apple-darwin9-g++"
export LD="$BINDIR/arm-apple-darwin9-gcc"
export AR="$BINDIR/arm-apple-darwin9-ar"
export RANLIB="$BINDIR/arm-apple-darwin9-ranlib"
#export LIBTOOL="$BINDIR/arm-apple-darwin9-libtool"

export CFLAGS="-isysroot $SIMULATOR -I$SIMULATOR/usr/include -I$SIMULATOR/usr/include/OpenGLES/ES2 -I$SIMULATOR/usr/include/OpenGL -g"
export CPPFLAGS="-isysroot $SIMULATOR -I$SIMULATOR/usr/include -g"

export LDFLAGS="-L$SIMULATOR/lib -L$SIMULATOR/usr/lib -L/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/lib/gcc/arm-apple-darwin10/4.2.1 -lstdc++ -lobjc -lpthread"

mkdir -p $TOOLS_SOURCE

export BOOST_SOURCE="${TOOLS_SOURCE}/boost"
export BOOST_PREFIX="${TOOLS_PRE}/boost"
#./boost.sh

export FREETYPE_PREFIX="${TOOLS_PRE}/freetype2"
#./freetype.sh

export CURL_PREFIX="${TOOLS_PRE}/curl"
#./curl.sh

export JPEGLIB_PREFIX="${TOOLS_PRE}/jpeglib"
#./jpeg.sh

export LIBPNG_PREFIX="${TOOLS_PRE}/libpng"
#./libpng.sh

export LIBEXPAT_PREFIX="${TOOLS_PRE}/libexpat"
#./libexpat.sh

export FONTCONFIG_PREFIX="${TOOLS_PRE}/fontconfig"
#./fontconfig.sh

export SDL_PREFIX="${TOOLS_PRE}/SDL"
./sdl.sh

export FFMPEG_PREFIX="${TOOLS_PRE}/ffmpeg"
#./ffmpeg.sh

export GIFLIB_PREFIX="${TOOLS_PRE}/giflib"
#./giflib.sh

export PIXMAN_PREFIX="${TOOLS_PRE}/pixman"
#./pixman.sh

export CAIRO_PREFIX="${TOOLS_PRE}/cairo"
#./cairo.sh

export AGG_PREFIX="${TOOLS_PRE}/agg"
#./agg.sh

export LIBXML2_PREFIX="${TOOLS_PRE}/libxml2"
#./libxml.sh

export LIBTOOL_PREFIX="${TOOLS_PRE}/libtool"
#./libtool.sh

#./gnash.sh
