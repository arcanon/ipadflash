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


#ifndef AQUA_OGL_GLUE_H
#define AQUA_OGL_GLUE_H

#include "aqua_glue.h"
#include <AGL/agl.h>


namespace gnash
{

class AquaOglGlue : public AquaGlue
{
  public:
    AquaOglGlue();
    virtual ~AquaOglGlue();

    bool init(int argc, char ***argv);
    render_handler* createRenderHandler();
    bool prepDrawingArea(int width, int height, AGLDrawable drawable);
    void setInvalidatedRegions(const InvalidatedRanges& /* ranges */);
    void render();
   private:
#ifdef FIX_I810_LOD_BIAS
    float _tex_lod_bias;
#endif
    AGLContext _context;
};
}

#endif /* AQUA_OGL_GLUE_H */
