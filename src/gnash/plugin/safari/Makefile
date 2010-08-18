CC = /home/ash/ipad-dev/iphonedevonlinux/toolchain/pre/bin/arm-apple-darwin9-g++
#CC=arm-apple-darwin9-gcc
LD = $(CC)
CXXLD = $(CC)

# Here comes your IP of the ipod/iphone.
# If you do a "make bundle" we build our
# application and copy via ssh the files to
# your ipod/iphone and sign them there.
#
# please make sure you have 'ldid' installed on your
# phone: 
#   apt-get install ldid
#
host = arm-apple-darwin9
host_alias = arm-apple-darwin9
host_cpu = arm
host_os = darwin9
host_vendor = apple

SRC=src/
IMG=img/

top_srcdir = ../..
top_builddir = $(top_srcdir)

CFLAGS = -isysroot /home/ash/ipad-dev/iphonedevonlinux/toolchain/sys -I/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/include -I/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/include/OpenGLES/ES2 -I/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/include/OpenGL -g  -dynamiclib   -W     -Wall     -Wcast-align     -Wcast-qual     -Wpointer-arith     -Wreturn-type    -I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/gui \
-I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/src/SDL/src/video/uikit/ \
-I$(top_srcdir)


LIBXML_CFLAGS = -I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/libxml2/include/libxml2
BOOST_CFLAGS = -I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/boost/include/boost-1_44

CURL_CFLAGS = /home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/curl/include
CURL_LIBS = -L/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/curl/lib

PTHREAD_CFLAGS = -D_THREAD_SAFE 
PTHREAD_LIBS = -L/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/lib -lpthread

am__append_28 = sdl-gnash
#am__append_29 = $(OPENGL_CFLAGS)
#am__append_30 = $(top_builddir)/backend/libgnashogl.la \
#	$(OPENGL_LIBS)

AGG_CFLAGS = -I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/agg/include/agg2
AGG_LIBS = -L/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/agg/lib -lagg

#am__append_31 = sdl_ogl_glue.cpp sdl_ogl_glue.h 
am__append_32 = $(AGG_CFLAGS)
am__append_33 = $(top_builddir)/backend/.libs/libgnashagg.a \
                $(AGG_LIBS)
	

am__append_3 = $(FFMPEG_LIBS)
am__append_4 = $(FFMPEG_CFLAGS)agg

FFMPEG_CFLAGS = -I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/ffmpeg/include
FFMPEG_LIBS = -L/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/ffmpeg/lib

RENDERER_CONFIG = agg
#RENDERER_LIBS =  $(top_builddir)/backend/.libs/libgnashagg.la

MEDIA_CONFIG = ffmpeg

SDL_CFLAGS = -I/home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/SDL/include/SDL
SDL_LIBS = /home/ash/ipad-dev/iphonedevonlinux/apps/gnash/pre/SDL/lib/libSDL.a

AM_CPPFLAGS = -I$(top_srcdir)/libamf -I$(top_srcdir)/libnet \
	-I$(top_srcdir)/server -I$(top_srcdir)/server/parser \
	-I$(top_srcdir)/server/vm -I$(top_srcdir)/libltdl \
	-I$(top_srcdir)/libbase -I$(top_srcdir)/backend \
	-I$(top_srcdir)/libgeometry -I$(top_srcdir)/libmedia \
	-I$(top_srcdir)/libmedia/ffmpeg -I$(top_srcdir)/libmedia/gst \
	-DLOCALEDIR=\"$(localedir)\" -DPLUGINSDIR=\"$(pluginsdir)\" \
	$(LIBXML_CFLAGS) $(DMALLOC_CFLApluginsdirGS) $(LIRC_CFLAGS) \
	$(BOOST_CFLAGS) $(PTHREAD_CFLAGS) $(NULL) $(am__append_4) \
	$(am__append_6) $(am__append_7) -DPKGDATADIR=\"$(pkgdatadir)\" \
	-DRENDERER_CONFIG=\"$(RENDERER_CONFIG)\" \
	-DMEDIA_CONFIG=\"$(MEDIA_CONFIG)\" \
	-DTARGET_CONFIG=\"$(target)\" -DCONFIG_CONFIG="\"$(shell cat \
	.configline)\"" -DCXXFLAGS="\"$(CXXFLAGS)\""

AM_LDFLAGS = $(GLIB_LIBS) $(LIBLTDL) $(CURL_LIBS) $(LIBXML_LIBS) \
	$(INCLTDL) $(LIRC_LIBS) $(MYSQL_LIBS) $(BOOST_LIBS) \
	$(PTHREAD_LIBS) $(NULL) $(am__append_3) $(am__append_5) \
	$(am__append_8)

GNASH_LIBS = \
	$(top_builddir)/libamf/.libs/libgnashamf-cvs.dylib \
	$(top_builddir)/libnet/.libs/libgnashnet.0.dylib \
	$(top_builddir)/libmedia/.libs/libgnashmedia-cvs.dylib \
	$(top_builddir)/server/.libs/libgnashserver-cvs.dylib \
	$(top_builddir)/libbase/.libs/libgnashbase-cvs.dylib

sdl_gnash_CPPFLAGS = -DGUI_SDL \
	-DGUI_CONFIG=\"SDL\" $(AM_CPPFLAGS) \
	$(SDL_CFLAGS) $(am__append_29) \
	$(am__append_32) $(am__append_35)

#sdl_gnash_LDFLAGS = $(LIBLTDL)
#sdl_gnash_DEPENDENCIES = .configline
sdl_gnash_LDADD = $(MYSQL_LIBS) $(GNASH_LIBS) \
	$(AM_LDFLAGS) $(SDL_LIBS) $(am__append_30) \
	$(am__append_33) $(am__append_36)

LIBTOOL = $(SHELL) $(top_builddir)/libtool

sdl_gnash_LDFLAGS = $(LIBLTDL)

sdl_gnash_LINK = $(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(sdl_gnash_LDFLAGS) $(LDFLAGS) -o $@

#sdl_gnash_LINK =$(CXXLD) $(AM_CXXFLAGS) \
#	$(CXXFLAGS) $(sdl_gnash_LDFLAGS) $(LDFLAGS) -o $@



LDFLAGS = -bind_at_load -lavdevice -lavformat -lswscale -lavutil -framework Foundation -framework UIKit -framework AudioToolbox -framework OpenGLES -framework QuartzCore -framework CoreFoundation -lcurl -lavcodec -L/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/lib -L/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/lib -L/home/ash/ipad-dev/iphonedevonlinux/toolchain/sys/usr/lib/gcc/arm-apple-darwin10/4.2.1 -dynamiclib -lstdc++ -lobjc -lpthread $(sdl_gnash_LDADD) -w  $(sdl_gnash_LDFLAGS) 

OBJS= plugin.lo ../../gui/sdl_agg_glue.lo

all:	libGnashPlugin.so

libGnashPlugin.so: $(OBJS) $(SDL_LIBS)
	echo $(OBJS)
	$(sdl_gnash_LINK) $(AM_LDFLAGS) -o $@ $^

%.lo:	%.mm
	$(LIBTOOL) --mode=compile $(CC) -c $(CFLAGS) $(sdl_gnash_CPPFLAGS) $< -o $@

%.lo:	%.cpp
	$(LIBTOOL) --mode=compile $(CC) -c $(CFLAGS) $(sdl_gnash_CPPFLAGS) $< -o $@

$(SDL_LIBS):
	pushd /home/ash/ipad-dev/iphonedevonlinux/apps/gnash/src/SDL
	make
	popd

deploy:
	scp libGnashPlugin.so root@ipad:"/System/Library/Internet\ Plug-Ins/MobileFlashView.webplugin/MobileFlashView"
	ssh root@ipad "killall -9 MobileSafari_"

clean:
	@rm -f $(OBJS)
	@rm -f *.lo
	@rm -f *.la
	@rm -f *.o
	@rm -rf .libs
	@rm -Rf libGnashPlugin.la



