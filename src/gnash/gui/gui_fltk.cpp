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


#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include "gui.h"
#include "GnashException.h"

#ifdef GUI_FLTK
#include "fltksup.h"
#endif

namespace gnash {

#ifdef GUI_FLTK
std::auto_ptr<Gui> createFLTKGui(unsigned long windowid, float scale, bool do_loop, unsigned int bit_depth)
{
	return std::auto_ptr<Gui>(new FltkGui(windowid, scale, do_loop, bit_depth));
}
#else // ! GUI_FLTK
std::auto_ptr<Gui> createFLTKGui(unsigned long , float , bool , unsigned int )
{
	throw GnashException("Support for FLTK gui was not compiled in");
}
#endif // ! GUI_FLTK

} // namespace gnash

