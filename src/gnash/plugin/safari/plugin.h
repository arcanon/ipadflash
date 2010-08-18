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

#ifndef __PLUGIN_H__
#define __PLUGIN_H__
 
#import <UIKit/UIKit.h>
// Needed to create the offscren buffer that ultimately accepts the rendering
// and is blitted to screen
#include <sdl_agg_glue.h>
#import <SDL_uikitopenglview.h>
#include "SDL_opengles.h"
#import <log.h>
//#include "URL.h"
#include "VM.h"
#include "movie_definition.h"
#include "movie_root.h"
#include "gnash.h"
#include "log.h"
#include "rc.h"
#include "Player.h"
#include "SystemClock.h"
#include "URL.h"

typedef std::map<std::string, std::string> VariableMap;

@interface GnashView : SDL_uikitopenglview
{
	NSDictionary* _flashVars;

    NSString *_flashURL;

    VariableMap _flashVars2;

    gnash::movie_definition* md;
    std::auto_ptr<gnash::media::sound_handler> _sound_handler;

    float movie_fps;

    gnash::movie_root *root;
    gnash::movie_instance* mi;

	id container;

    int _width, _height;

    gnash::SdlAggGlue sdlaggglue;
}

@property (nonatomic, retain) id container;

@end
 
#endif // __PLUGIN_H__
