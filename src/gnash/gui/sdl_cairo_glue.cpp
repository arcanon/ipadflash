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

#include "sdl_cairo_glue.h"
#include "log.h"
#include "render_handler_cairo.h"

using namespace std;


namespace gnash
{

SdlCairoGlue::SdlCairoGlue()
{
//    GNASH_REPORT_FUNCTION;
}

SdlCairoGlue::~SdlCairoGlue()
{
//    GNASH_REPORT_FUNCTION;
    cairo_surface_destroy(_cairo_surface);
    cairo_destroy (_cairo_handle);
    SDL_FreeSurface(_sdl_surface);
    SDL_FreeSurface(_screen);
    delete [] _render_image;
}

bool
SdlCairoGlue::init(int argc, char** argv[])
{
//    GNASH_REPORT_FUNCTION;
    return true;
}


render_handler*
SdlCairoGlue::createRenderHandler(int depth)
{
//    GNASH_REPORT_FUNCTION;
    _bpp = depth;

    return renderer::cairo::create_handler();

}


bool
SdlCairoGlue::prepDrawingArea(int width, int height, boost::uint32_t sdl_flags)
{
    _screen = SDL_SetVideoMode(width, height, _bpp, sdl_flags | SDL_SWSURFACE);

    if (!_screen) {
        fprintf(stderr, "SDL_SetVideoMode() failed.\n");
        exit(1);
    }
    
    int stride=width * 4;

    _render_image = new unsigned char[stride * height];
    // XXX is there a need for zeroing out _render_image?

    _cairo_surface =
      cairo_image_surface_create_for_data (_render_image, CAIRO_FORMAT_ARGB32,
                                           width, height, stride);

    _cairo_handle = cairo_create(_cairo_surface);

    renderer::cairo::set_handle(_cairo_handle);

    boost::uint32_t rmask, gmask, bmask, amask;

    rmask = 0x00ff0000;
    gmask = 0x0000ff00;
    bmask = 0x000000ff;
    amask = 0xff000000;

    _sdl_surface = SDL_CreateRGBSurfaceFrom((void *) _render_image, width, height,
                                           _bpp, stride, rmask, gmask, bmask, amask);
    assert(_sdl_surface);

    return true;
}

void
SdlCairoGlue::render()
{
//    GNASH_REPORT_FUNCTION;
    SDL_BlitSurface(_sdl_surface, NULL, _screen, NULL);
    SDL_UpdateRect (_screen, 0, 0, 0, 0);
}


} // namespace gnash
