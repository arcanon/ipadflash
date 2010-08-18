// sdl_agg_glue.cpp:  Glue between SDL and Anti-Grain Geometry, for Gnash.
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

#include "sdl_agg_glue.h"
#include "log.h"
#include "render_handler.h"
#include "render_handler_agg.h"
#include <cerrno>
#include <ostream>

using namespace std;

namespace gnash
{

SdlAggGlue::SdlAggGlue()
	:
_sdl_surface(NULL),
_offscreenbuf(NULL),
_screen(NULL),
_agg_renderer(NULL)
{
//    GNASH_REPORT_FUNCTION;
}

SdlAggGlue::~SdlAggGlue()
{
//    GNASH_REPORT_FUNCTION;
    SDL_FreeSurface(_sdl_surface);
    SDL_FreeSurface(_screen);
    delete [] _offscreenbuf;
}

bool
SdlAggGlue::init(int /*argc*/, char*** /*argv*/)
{
//    GNASH_REPORT_FUNCTION;

    return true;
}


render_handler*
SdlAggGlue::createRenderHandler(int bpp)
{
//    GNASH_REPORT_FUNCTION;

    _bpp = bpp;

    switch (_bpp) {
      case 32:
        _agg_renderer = create_render_handler_agg("RGBA32");
        break;
      case 24:
        _agg_renderer = create_render_handler_agg("RGB24");
        break;
      case 16:
        _agg_renderer = create_render_handler_agg("RGBA16");
        break;
      default:
        log_error (_("AGG's bit depth must be 16, 24 or 32 bits, not %d."), _bpp);
        abort();
    }
    return _agg_renderer;
}


bool
SdlAggGlue::prepDrawingArea(int width, int height, boost::uint32_t sdl_flags, void * win, void *oglview)
{
    int depth_bytes = _bpp / 8;  // TODO: <Udo> is this correct? Gives 1 for 15 bit modes!

    assert(_bpp % 8 == 0);

    fprintf (stderr,"prep win %X %X", win, oglview);

    _screen = SDL_SetVideoMode2(width, height, _bpp, sdl_flags | SDL_SWSURFACE, win, oglview);

    if (!_screen) {
        fprintf (stderr,"SDL_SetVideoMode() failed for SdlAggGlue.\n");
        exit(1);
    }

    fprintf(stderr, "prep drawing bpp %d\n", _bpp);

    int stride = width * depth_bytes;

    boost::uint32_t rmask, gmask, bmask, amask;

    switch(_bpp) {
      case 32: // RGBA32
        rmask = 0xFF;
        gmask = 0xFF << 8;
        bmask = 0xFF << 16;
        amask = 0xFF << 24;
        break;
      case 24: // RGB24
        rmask = 0xFF;
        gmask = 0xFF << 8;
        bmask = 0xFF << 16;
        amask = 0;
        break;
      case 16: // RGB565: 5 bits for red, 6 bits for green, and 5 bits for blue
        rmask = 0x1F << 11;
        gmask = 0x3F << 5;
        bmask = 0x1F;
        amask = 0;
        break;
      default:
        abort();
    }

#define CHUNK_SIZE (100 * 100 * depth_bytes)

    int bufsize = static_cast<int>(width * height * depth_bytes / CHUNK_SIZE + 1) * CHUNK_SIZE;

    _offscreenbuf = new unsigned char[bufsize];

    log_debug (_("SDL-AGG: %i byte offscreen buffer allocated"), bufsize);


    // Only the AGG renderer has the function init_buffer, which is *not* part of
    // the renderer api. It allows us to change the renderers movie size (and buffer
    // address) during run-time.
    render_handler_agg_base * renderer =
      static_cast<render_handler_agg_base *>(_agg_renderer);
    renderer->init_buffer(_offscreenbuf, bufsize, width, height,
      width*((_bpp+7)/8));


    _sdl_surface = SDL_CreateRGBSurfaceFrom((void *) _offscreenbuf, width, height,
                                           _bpp, stride, rmask, gmask, bmask, amask);
    assert(_sdl_surface);

//    SDL_Surface *SDL_VideoSurface =
//            SDL_CreateRGBSurfaceFrom(NULL, width, height, _bpp, 0, rmask, gmask, bmask, amask);
//    if (!SDL_VideoSurface) {
//        return NULL;
//    }
//    _screen = SDL_VideoSurface;

    

    return true;
}

void
SdlAggGlue::render()
{
    rect bounds;
    bounds.set_world();
    
    _agg_renderer->set_invalidated_region(bounds);

  	int res = SDL_BlitSurface(_sdl_surface, 0, _screen, 0);

    //log_debug (_("SDL-AGG: render 1, result %d"), res);

	// Update the entire screen

    SDL_UpdateRect(_screen, 0, 0, 0, 0);
}

void
SdlAggGlue::render(int minx, int miny, int maxx, int maxy)
{

    log_debug (_("SDL-AGG: render 2"));
	// Update only the invalidated rectangle
	SDL_Rect clip = { minx, miny, maxx - minx, maxy - miny };
	SDL_SetClipRect(_screen, &clip);
	SDL_BlitSurface(_sdl_surface, 0, _screen, 0);
	SDL_UpdateRect(_sdl_surface, clip.x, clip.y, clip.w, clip.h);
}

} // namespace gnash
