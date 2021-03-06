## Process this file with automake to generate Makefile.in
# 
#   Copyright (C) 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

# 
# build gtk-gnash, kde-gnash, sdl-gnash, fltk-gnash, aqua-gnash, fb-gnash, riscos-gnash
# and dump-gnash
#

AUTOMAKE_OPTIONS = -Wno-portability

localedir = $(datadir)/locale

# this is where Gnash plugins get installed
pluginsdir = $(libdir)/gnash/plugins

# noinst_LTLIBRARIES = libgnashgui.la
BUILT_SOURCES = .configline

EXTRA_DIST = gnash.in klash.moc.in

noinst_LTLIBRARIES = 
AM_CPPFLAGS = \
        -I$(top_srcdir)/libamf \
        -I$(top_srcdir)/libnet \
        -I$(top_srcdir)/server \
        -I$(top_srcdir)/server/parser \
        -I$(top_srcdir)/server/vm \
	-I$(top_srcdir)/libltdl \
        -I$(top_srcdir)/libbase \
        -I$(top_srcdir)/backend \
        -I$(top_srcdir)/libgeometry \
	 	-I$(top_srcdir)/libmedia \
		-I$(top_srcdir)/libmedia/ffmpeg \
		-I$(top_srcdir)/libmedia/gst \
       -DLOCALEDIR=\"$(localedir)\" \
	-DPLUGINSDIR=\"$(pluginsdir)\" \
        $(LIBXML_CFLAGS) \
        $(DMALLOC_CFLAGS) \
	$(LIRC_CFLAGS) \
	$(BOOST_CFLAGS) \
        $(PTHREAD_CFLAGS) \
	$(NULL)

GTK_CFLAGS = \
	$(GTK2_CFLAGS) \
	$(GLEXT_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(PANGO_CFLAGS) \
	$(ATK_CFLAGS) \
	$(CAIRO_CFLAGS)

GTK_LIBS =  \
	$(GTK2_LIBS) \
	$(GLEXT_LIBS) \
	$(CAIRO_LIBS) \
	$(PANGO_LIBS) \
	$(ATK_LIBS) \
	$(GLIB_LIBS) \
	$(EXPAT_LIBS)

DUMP_CFLAGS =

DUMP_LIBS =

# The following will be filled up later
GUI_CPPFLAGS = 

## WARNING: make sure GLIB_LIBS appears first
## See: http://lists.gnu.org/archive/html/gnash-dev/2006-07/msg00076.html
AM_LDFLAGS =  \
	$(GLIB_LIBS) \
	$(LIBLTDL) \
	$(CURL_LIBS) \
	$(LIBXML_LIBS) \
	$(INCLTDL) \
	$(LIRC_LIBS) \
	$(MYSQL_LIBS) \
	$(BOOST_LIBS) \
	$(PTHREAD_LIBS) \
	$(NULL)

GNASH_LIBS = \
	$(top_builddir)/libamf/libgnashamf.la \
	$(top_builddir)/libnet/libgnashnet.la \
	$(top_builddir)/libmedia/libgnashmedia.la \
	$(top_builddir)/server/libgnashserver.la \
	$(top_builddir)/libbase/libgnashbase.la

# Only build loadable modules unless statically linking
# if DYNAMIC_GUI # make shared libraries suitable for dlopen().
# if  ENABLE_SHARED
#      LIB_FLAGS = -module -avoid-version -no-undefined
# else
#      LIB_FLAGS = -release $(VERSION)
# endif # ENABLE_SHARED
# else # make shared libraries suitable for linking.
      LIB_FLAGS = -release $(VERSION)
# endif # DYNAMIC_GUI

# AQUA Toolkit
if BUILD_AQUA_GUI
#OS X stuff goes here...
GUI_CPPFLAGS += -DGUI_AQUA

bundle_name = Gnash.app


if  HAVE_OPENGL
noinst_LTLIBRARIES += libgnashaqua_ogl.la
libgnashaqua_ogl_la_CPPFLAGS = $(AM_CPPFLAGS) \
	$(OPENGL_CFLAGS)
libgnashaqua_ogl_la_SOURCES = \
	aqua_ogl_glue.cpp aqua_ogl_glue.h \
	aqua.cpp aquasup.h aqua_glue.h
libgnashaqua_ogl_la_LDFLAGS = $(LIB_FLAGS)
libgnashaqua_ogl_la_LIBADD = \
	$(top_builddir)/backend/libgnashogl.la 
endif # HAVE_OPENGL
endif # BUILD_AQUA_GUI

if USE_FFMPEG_ENGINE
AM_LDFLAGS += $(FFMPEG_LIBS)
AM_CPPFLAGS += $(FFMPEG_CFLAGS) 
endif

if USE_MAD_ENGINE
AM_LDFLAGS += $(MAD_LIBS)
AM_CPPFLAGS += $(MAD_CFLAGS)
endif

if USE_SOUND_GST
AM_CPPFLAGS += $(GSTREAMER_CFLAGS)
AM_LDFLAGS += $(GSTREAMER_LIBS) 
endif

# if USE_GUI_FLTK
#  FLTK_SRCS = fltk.cpp fltksup.h $(FLTK_AGG_SRCS) $(FLTK_CAIRO_SRCS)
#  AM_CPPFLAGS += $(FLTK2_CFLAGS) $(X11_CFLAGS)
#  AM_LDFLAGS += $(FLTK2_LIBS) $(XFT_LIBS) $(X11_LIBS)
# else
#  FLTK_SRCS = 
# endif

AM_CPPFLAGS += -DPKGDATADIR=\"$(pkgdatadir)\"
AM_CPPFLAGS += -DRENDERER_CONFIG=\"$(RENDERER_CONFIG)\"
AM_CPPFLAGS += -DMEDIA_CONFIG=\"$(MEDIA_CONFIG)\"
AM_CPPFLAGS += -DTARGET_CONFIG=\"$(target)\"
AM_CPPFLAGS += -DCONFIG_CONFIG="\"$(shell cat .configline)\""
AM_CPPFLAGS += -DCXXFLAGS="\"$(CXXFLAGS)\"" 

dist_images_DATA = images/gnash_128_96.ico images/GnashG.png
#dist_images_DATA = images/gnash_G_v3.ico
imagesdir = $(pkgdatadir)

bin_SCRIPTS = gnash
bin_PROGRAMS = 

gnash: gnash.in
	cp $< $@
	chmod +x $@

GUI_SRCS = gnash.cpp \
	gui.cpp gui.h \
	Player.cpp Player.h \
	NullGui.cpp NullGui.h

if BUILD_AQUA_GUI
BUILT_SOURCES += bundle
EXTRA_DIST += Gnash.app am-frag
endif

# Build the QTOPIA gui
if BUILD_QTOPIA_GUI
include $(srcdir)/am-frag/qtopia.am
endif

# Build the KDE gui
if BUILD_KDE_GUI
BUILT_SOURCES += klash.moc 
include $(srcdir)/am-frag/kde.am
endif

# Build the SDL gui
if BUILD_SDL_GUI
include $(srcdir)/am-frag/sdl.am
endif

# Build the FLTK gui
if BUILD_FLTK_GUI
include $(srcdir)/am-frag/fltk.am
endif

# Build the AQUA gui
if BUILD_AQUA_GUI
include $(srcdir)/am-frag/aqua.am
endif

# Build the GTK gui
if BUILD_GTK_GUI
include $(srcdir)/am-frag/gtk.am
endif

# if ALP				# Access Linux Platform
# gtk_gnash_LDFLAGS += "-shared -Xlinker -z -Xlinker defs $(GTK2_LIBS)"
# gtk_gnash_LDADD += $(ALP_LIBS)
# endif
#
# Build the GTK/ALP gui
if BUILD_ALP_GUI
include $(srcdir)/am-frag/alp.am
endif

# Build the GTK/HILDON gui
if BUILD_HILDON_GUI
include $(srcdir)/am-frag/hildon.am
endif

# Build the DUMP gui
if BUILD_DUMP_GUI
include $(srcdir)/am-frag/dump.am
endif

# Build the FB gui
if BUILD_FB_GUI
include $(srcdir)/am-frag/fb.am
endif

# Build the RISCOS gui
if BUILD_RISCOS_GUI
include $(srcdir)/am-frag/riscos.am
endif

CLEANFILES = klash.moc .configline gnash

bundle: Info.plist
	@echo " Building: $(bundle_name)" 
	rm -rf $(bundle_name)/Contents
	mkdir -p $(bundle_name)/Contents/MacOS
	mkdir -p $(bundle_name)/Contents/Resources
	echo "APPL????" > $(bundle_name)/Contents/PkgInfo
	cp Info.plist $(bundle_name)/Contents/Info.plist
	cp -f .libs/aqua-gnash $(bundle_name)/Contents/MacOS/Gnash
	cp -f images/GnashG.icns $(bundle_name)/Contents/Resources/


klash.moc: $(srcdir)/kdesup.h
	@if test x"$(MOC)" != x; then \
	  echo "Generating MOC file..."; \
	  $(MOC) $(srcdir)/kdesup.h -o klash.moc; \
	else  \
	  echo "WARNING: Install QT's moc tool! Linking to default MOC file"; \
	  ln -sf $(srcdir)/klash.moc.in klash.moc; \
	fi

.configline: ../config.log
	-@rm -f .configline
	@head ../config.log | grep " .*/configure " | sed -e 's:^  . .*configure ::' > .configline

#noinst_PROGRAMS = guitest # klash flak
#guitest_SOURCES = guitest.cpp #fltk.cpp #fltksup.h fltksupp.h gnash.cpp gtk.cpp 
#	gtksup.h gui.cpp gui.h menu.cpp menu.h sdl.cpp sdlsup.h [21~sdlsupp.h
#guitest_LDADD = $(gnash_LDADD)

# Rebuild with GCC 4.x Mudflap support
mudflap:
	@echo "Rebuilding with GCC Mudflap support"
	$(MAKE) CXXFLAGS="$(CXXFLAGS) -fmudflap" LDFLAGS="$(LDFLAGS) -lmudflap"

clean-hook:
	-rm -f core.* $(CLEANFILES)

# if DYNAMIC_GUI
# install-pluginsLTLIBRARIES: $(plugins_LTLIBRARIES)
# 	test -d "$(DESTDIR)$(plugindir)" || $(mkinstalldirs) "$(DESTDIR)$(plugindir)"
# 	for i in $(plugins_LTLIBRARIES); do \
# 	  $(LIBTOOL) --mode=install $(INSTALL) $(INSTALL_STRIP_FLAG) $$i "$(DESTDIR)$(plugindir)/$$i" ; \
# 	done
# endif

