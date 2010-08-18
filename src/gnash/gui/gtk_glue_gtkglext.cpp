// gtk_glue_gtkglext.cpp:  Gnome ToolKit glue of some sort, for Gnash.
//
//   Copyright (C) 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//


#include "gtk_glue_gtkglext.h"
#include "log.h"

using namespace std;

namespace gnash
{

GtkGlExtGlue::GtkGlExtGlue()
#ifdef FIX_I810_LOD_BIAS
  : _tex_lod_bias(-1.2f)
#endif
{
//    GNASH_REPORT_FUNCTION;
}

GtkGlExtGlue::~GtkGlExtGlue()
{
//    GNASH_REPORT_FUNCTION;
    if (_glconfig) {
        g_object_unref (G_OBJECT (_glconfig));
        _glconfig = NULL;
    }
    
    GdkGLContext *glcontext = gtk_widget_get_gl_context (_drawing_area);
    if (glcontext) {
       g_object_unref (G_OBJECT (glcontext));
       glcontext = NULL;
    }
}

bool
GtkGlExtGlue::init(int argc, char** argv[])
{
//    GNASH_REPORT_FUNCTION;
#ifdef FIX_I810_LOD_BIAS
    int c = getopt (argc, *argv, "m:");
    if (c == 'm') {
      _tex_lod_bias = (float) atof(optarg);
    }
#endif

    gtk_gl_init (&argc, argv);

    gint major, minor;
    gdk_gl_query_version (&major, &minor);
    log_debug (_("OpenGL extension version - %d.%d"),
              (int)major, (int)minor);

    GdkGLConfigMode glcmode = (GdkGLConfigMode)(GDK_GL_MODE_RGB |
                                                GDK_GL_MODE_STENCIL |
                                                GDK_GL_MODE_DOUBLE |
                                                GDK_GL_MODE_ACCUM);
    _glconfig = gdk_gl_config_new_by_mode (glcmode);

    if (!_glconfig) {
      log_error (_("Cannot find the double-buffered visual.\n"
      		   "Trying single-buffered visual."));

      glcmode = (GdkGLConfigMode)(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH);
      _glconfig = gdk_gl_config_new_by_mode (glcmode);
      if (!_glconfig) {
        log_error (_("No appropriate OpenGL-capable visual found."));
        gtk_main_quit(); // XXX
      } else {
        log_debug (_("Got single-buffered visual."));
      }
    } else {
      log_debug (_("Got double-buffered visual."));
    }
    
    return true;
}


void
GtkGlExtGlue::prepDrawingArea(GtkWidget *drawing_area)
{
//    GNASH_REPORT_FUNCTION;
    _drawing_area = drawing_area;
    gtk_widget_set_gl_capability(_drawing_area, _glconfig,
                                 NULL, TRUE, GDK_GL_RGBA_TYPE);
}

render_handler*
GtkGlExtGlue::createRenderHandler()
{
//    GNASH_REPORT_FUNCTION;
    GdkGLContext *glcontext = gtk_widget_get_gl_context (_drawing_area);
    GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (_drawing_area);

    // Attach our OpenGL context to the drawing_area.
    gdk_gl_drawable_make_current(gldrawable, glcontext);

    render_handler* renderer = create_render_handler_ogl();

#ifdef FIX_I810_LOD_BIAS
    glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, _tex_lod_bias);
#endif

    return renderer;
}

void
GtkGlExtGlue::render()
{
//    GNASH_REPORT_FUNCTION;
    GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (_drawing_area);
    if (gdk_gl_drawable_is_double_buffered (gldrawable)) {
        gdk_gl_drawable_swap_buffers (gldrawable);
    } else {
      glFlush();
    }
}

void
GtkGlExtGlue::configure(GtkWidget *const widget, GdkEventConfigure *const event)
{
    GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
    GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);
    if (gdk_gl_drawable_make_current(gldrawable, glcontext)) {
        glViewport (event->x, event->y, event->width, event->height);
    }
}


} // namespace gnash
