// gui.cpp:  Top level GUI for flash player, for Gnash.
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

#include "sprite_instance.h"
#include "gui.h"
#include "render.h"  // debug
#include "render_handler.h"
#include "sound_handler.h"
#include "gnash.h" // for get_sound_handler
#include "movie_root.h"
#include "VM.h"

#ifdef GNASH_FPS_DEBUG
#include "ClockTime.h"
#include <boost/format.hpp>
#endif

#include <cstdio>
#include <cstring>
#include <algorithm> // std::max, std::min

#ifdef SKIP_RENDERING_IF_LATE
#include "WallClockTimer.h"
#endif

/// Define this to make sure each frame is fully rendered from ground up
/// even if no motion has been detected in the movie.
//#define FORCE_REDRAW 1

/// Define this if you want to debug the *detection* of region updates only.
/// This will disable region updates for the backend (GUI+renderer) completely 
/// so that only the last region (red frame) will be visible. However, this 
/// slows down rendering as each frame is fully re-rendered. If you want to 
/// debug the GUI part, however (see if blitting the region works), then you 
/// probably won't define this.
#ifndef DISABLE_REGION_UPDATES_DEBUGGING 
//#define REGION_UPDATES_DEBUGGING_FULL_REDRAW 1
#endif 

#ifndef DISABLE_REGION_UPDATES_DEBUGGING
// a runtime check would make the { x; } block conditionally executed
#define IF_DEBUG_REGION_UPDATES(x) { if (_showUpdatedRegions) { x } }
#else
#define IF_DEBUG_REGION_UPDATES(x) 
#endif

// Define this to have gnash print the mouse pointer coordinates
// as the mouse moves. See also ENABLE_KEYBOARD_MOUSE_MOVEMENTS
// to have more control over mouse pointer.
//
//#define DEBUG_MOUSE_COORDINATES 1


namespace gnash {

Gui::Gui() :
    _loop(true),
    _xid(0),
    _width(1),
    _height(1),
    _depth(16),
    _interval(0),
    _renderer(NULL),
    _redraw_flag(true),
    _fullscreen(false),
    _mouseShown(true),
    _maxAdvances(0),
    _xscale(1.0f),
    _yscale(1.0f),
    _xoffset(0),
    _yoffset(0)
#ifdef GNASH_FPS_DEBUG
    ,fps_counter(0)
    ,fps_counter_total(0)
    ,fps_timer(0)
    ,fps_timer_interval(0.0)
#endif
#ifdef SKIP_RENDERING_IF_LATE
    ,estimatedDisplayTime(0) // milliseconds (will grow later..)
#endif // SKIP_RENDERING_IF_LATE
    ,_movieDef(0)
    ,_stage(0)
    ,_stopped(false)
    ,_started(false)
    ,_showUpdatedRegions(false)
#ifdef ENABLE_KEYBOARD_MOUSE_MOVEMENTS 
    ,_xpointer(0)
    ,_ypointer(0)
    ,_keyboardMouseMovements(true) // TODO: base default on gnashrc or always false and provide menu item to toggle
    ,_keyboardMouseMovementsStep(1)
#endif
{

}

Gui::Gui(unsigned long xid, float scale, bool loop, unsigned int depth)
	:
    _loop(loop),
    _xid(xid),
    _width(1),
    _height(1),
    _depth(depth),
    _interval(0),
    _renderer(NULL),
    _redraw_flag(true),
    _fullscreen(false),
    _mouseShown(true),
    _maxAdvances(0),
    _xscale(scale),
    _yscale(scale),
    _xoffset(0), // TODO: x and y offset will need update !
    _yoffset(0)
#ifdef GNASH_FPS_DEBUG
    ,fps_counter(0)    
    ,fps_counter_total(0)    
    ,fps_timer(0)
    ,fps_timer_interval(0.0)
#endif        
#ifdef SKIP_RENDERING_IF_LATE
    ,estimatedDisplayTime(0) // milliseconds (will grow later..)
#endif // SKIP_RENDERING_IF_LATE
    ,_movieDef(0)
    ,_stage(0)
    ,_stopped(false)
    ,_started(false)
    ,_showUpdatedRegions(false)
#ifdef ENABLE_KEYBOARD_MOUSE_MOVEMENTS 
    ,_xpointer(0)
    ,_ypointer(0)
    ,_keyboardMouseMovements(true) // TODO: base default on gnashrc or always false and provide menu item to toggle
    ,_keyboardMouseMovementsStep(1)
#endif
{
}

Gui::~Gui()
{

    delete _renderer;
#ifdef GNASH_FPS_DEBUG
    std::cerr << "Total frame advances: " << fps_counter_total << std::endl;
#endif
}

void
Gui::setFullscreen()
{
    log_unimpl(_("Fullscreen not yet supported in this GUI"));
}

void
Gui::unsetFullscreen()
{
    log_unimpl(_("Fullscreen not yet supported in this GUI"));
}

bool
Gui::showMouse(bool /* show */)
{
	LOG_ONCE(log_unimpl(_("Mouse show/hide not yet supported in this GUI")));
   	return true;
}

void
Gui::showMenu(bool /* show */)
{
	LOG_ONCE(log_unimpl(_("menushow not yet supported in this GUI")));
}

void
Gui::toggleFullscreen()
{
	if (_fullscreen) {
		unsetFullscreen();
	}
	else {
		setFullscreen();
	} 
}

void
Gui::menu_restart()
{
	_stage->reset();
	_started = false;
	start();
}

void
Gui::updateStageMatrix()
{
	if ( ! VM::isInitialized() )
	{
		// When VM initializes, we'll get a call to resize_view, which
		// would call us again.
		//log_debug("Can't update stage matrix till VM is initialized");
		return;
	}

	assert(_stage); // when VM is initialized this should hold

	float swfwidth = _movieDef->get_width_pixels();
	float swfheight = _movieDef->get_height_pixels();

	// Fetch scale mode
	movie_root::ScaleMode scaleMode = _stage->getStageScaleMode();
	switch (scaleMode)
	{
		case movie_root::noScale:
			_xscale = _yscale = 1.0f;
			break;

		case movie_root::showAll:
		{
		
			// set new scale value ( user-pixel / pseudo-pixel ). Do
			// not divide by zero, or we end up with an invalid
			// stage matrix that returns nan values.			
			_xscale = (swfwidth == 0.0f) ? 1.0f : _width / swfwidth;
			_yscale = (swfheight == 0.0f) ? 1.0f : _height / swfheight;
			
			// Scale proportionally, using smallest scale
			if (_xscale < _yscale) _yscale = _xscale;
			else if (_yscale < _xscale) _xscale = _yscale;

			break;
		}

		case movie_root::noBorder:
		{

			// set new scale value ( user-pixel / pseudo-pixel )
			_xscale = (swfwidth == 0.0f) ? 1.0f : _width / swfwidth;
			_yscale = (swfheight == 0.0f) ? 1.0f : _height / swfheight;
			
			// Scale proportionally, using biggest scale
			if (_xscale > _yscale) _yscale = _xscale;
			else if (_yscale > _xscale) _xscale = _yscale;

			break;
		}

		case movie_root::exactFit:
		{
			// NOTE: changing aspect ratio is valid!
			_xscale = (swfwidth == 0.0f) ? 1.0f : _width / swfwidth;
			_yscale = (swfheight == 0.0f) ? 1.0f : _height / swfheight;
			//LOG_ONCE( log_unimpl("Stage.scaleMode=exactFit") );
			break;
		}

		default:
		{
			log_error("Invalid scaleMode %d", scaleMode);
			break;
		}
	}

	_xoffset=0;
	_yoffset=0;

	// Fetch align mode
	movie_root::StageAlign align = _stage->getStageAlignment();
	movie_root::StageHorizontalAlign halign = align.first;
	movie_root::StageVerticalAlign valign = align.second;

	// Handle horizontal alignment
	switch ( halign )
	{
		case movie_root::STAGE_H_ALIGN_L:
		{
			// _xoffset=0 is fine
			break;
		}

		case movie_root::STAGE_H_ALIGN_R:
		{
			// Offsets in pixels
			float defWidth = swfwidth *= _xscale;
			float diffWidth = _width-defWidth;
			_xoffset = diffWidth;
			break;
		}

		case movie_root::STAGE_V_ALIGN_C:
		{
			// Offsets in pixels
			float defWidth = swfwidth *= _xscale;
			float diffWidth = _width-defWidth;
			_xoffset = diffWidth/2.0;
			break;
		}

		default:
		{
			log_error("Invalid horizontal align %d", valign);
			break;
		}
	}

	// Handle vertical alignment
	switch ( valign )
	{
		case movie_root::STAGE_V_ALIGN_T:
		{
			// _yoffset=0 is fine
			break;
		}

		case movie_root::STAGE_V_ALIGN_B:
		{
			float defHeight = swfheight *= _yscale;
			float diffHeight = _height-defHeight;
			_yoffset = diffHeight;
			break;
		}

		case movie_root::STAGE_V_ALIGN_C:
		{
			float defHeight = swfheight *= _yscale;
			float diffHeight = _height-defHeight;
			_yoffset = diffHeight/2.0;
			break;
		}

		default:
		{
			log_error("Invalid vertical align %d", valign);
			break;
		}
	}

	//log_debug("updateStageMatrix: scaleMode:%d, valign:%d, halign:%d", scaleMode, valign, halign);

	// TODO: have a generic set_matrix ?
	if ( _renderer ) {
		_renderer->set_scale(_xscale, _yscale);
		_renderer->set_translation(_xoffset, _yoffset);
	}
	else
	{
		//log_debug("updateStageMatrix: could not signal updated stage matrix to renderer (no renderer registered)");
	}

	// trigger redraw
	//_redraw_flag |= (_width!=width) || (_height!=height);
	_redraw_flag = true; // this fixes bug #21971
}


void
Gui::resize_view(int width, int height)
{
	GNASH_REPORT_FUNCTION;

	assert(width>0);
	assert(height>0);

	if ( VM::isInitialized() )
	{

		if ( _stage && _started )
		{
			_stage->set_display_viewport(0, 0, width, height);
		}

	}

	_width = width;
	_height = height;
	_validbounds.setTo(0, 0, _width-1, _height-1);

	updateStageMatrix();

	if ( _stage && _started ) display(_stage);
}

void
Gui::menu_quit()
{
    quit();
}

void
Gui::menu_play()
{
    play();
}

void
Gui::menu_pause()
{
    pause();
}

void
Gui::menu_stop()
{
    stop();
}

void
Gui::menu_step_forward()
{
	movie_root* m = get_current_root();
	m->goto_frame(m->get_current_frame()+1);
}

void
Gui::menu_step_backward()
{
	movie_root* m = get_current_root();
	m->goto_frame(m->get_current_frame()-1);
}

void
Gui::menu_jump_forward()
{
	movie_root* m = get_current_root();
	m->goto_frame(m->get_current_frame()+10);
}

void
Gui::menu_jump_backward()
{
	movie_root* m = get_current_root();
	m->goto_frame(m->get_current_frame()-10);
}

void
Gui::menu_toggle_sound()
{

    media::sound_handler* s = get_sound_handler();

    if (!s)
       return;

    if (s->is_muted()) {
       s->unmute();
    } else {
       s->mute();
    }
}


void
Gui::notify_mouse_moved(int ux, int uy) 
{
	movie_root* m = _stage;

	if ( ! _started ) return;

	if ( _stopped ) return;

	// A stage pseudopixel is user pixel / _xscale wide
	float x = (ux-_xoffset) / _xscale;

	// A stage pseudopixel is user pixel / _xscale high
	float y = (uy-_yoffset) / _yscale;

#ifdef DEBUG_MOUSE_COORDINATES
	log_debug(_("mouse @ %d,%d"), x, y);
#endif

	if ( m->notify_mouse_moved(x, y) )
	{
		// any action triggered by the
		// event required screen refresh
		display(m);
	}
    
	character* activeEntity = m->getActiveEntityUnderPointer();
	if ( activeEntity )
	{
		if ( activeEntity->isSelectableTextField() )
		{
			setCursor(CURSOR_INPUT);
		}
		else if ( activeEntity->allowHandCursor() )
		{
			setCursor(CURSOR_HAND);
		}
		else
		{
			setCursor(CURSOR_NORMAL);
		}
	}
	else
	{
		setCursor(CURSOR_NORMAL);
	}

#ifdef ENABLE_KEYBOARD_MOUSE_MOVEMENTS
	_xpointer = ux;
	_ypointer = uy;
#endif


}

void
Gui::notify_mouse_clicked(bool mouse_pressed, int mask) 
{
	movie_root* m = _stage;
	assert(m);

    if ( ! _started ) return;

    if ( _stopped ) return;

	if ( m->notify_mouse_clicked(mouse_pressed, mask) )
	{
		// any action triggered by the
		// event required screen refresh
		display(m);
	}
}

void
Gui::refreshView()
{
    movie_root* m = _stage;

    if ( ! _started ) return;

    assert(m);
    _redraw_flag=true;
    display(m);
}


void
Gui::notify_key_event(gnash::key::code k, int modifier, bool pressed) 
{
	movie_root* m = _stage;

	/* Handle GUI shortcuts */
	if (pressed)
	{
		if (k == gnash::key::ESCAPE)
		{
			if (isFullscreen())
			{
				unsetFullscreen();
			}
		}
		
		if (modifier & gnash::key::GNASH_MOD_CONTROL)
		{
			switch(k)
			{
				case gnash::key::r:
				case gnash::key::R:
					menu_restart();
					break;
				case gnash::key::p:
				case gnash::key::P:
					menu_pause();
					break;
				case gnash::key::l:
				case gnash::key::L:
					refreshView();
					break;
				case gnash::key::q:
				case gnash::key::Q:
				case gnash::key::w:
				case gnash::key::W:
					menu_quit();
					break;
				case gnash::key::f:
				case gnash::key::F:
					toggleFullscreen();
					break;
				case gnash::key::RIGHT_BRACKET:
					menu_step_forward();
					break;
				case gnash::key::LEFT_BRACKET:
					menu_step_backward();
					break;
				default:
					break;
			}

#ifdef ENABLE_KEYBOARD_MOUSE_MOVEMENTS
			if ( _keyboardMouseMovements )
			{
				int step = _keyboardMouseMovementsStep; 
				if (modifier & gnash::key::GNASH_MOD_SHIFT) step*=5; // x5 if SHIFT is pressed
				switch(k)
				{
					case gnash::key::UP:
					{
						int newx = _xpointer;
						int newy = _ypointer-step;
						//log_debug("(theoretically) From %d,%d to %d,%d (step %d)", _xpointer, _ypointer, newx, newy, step);
						if ( newy < 0 ) newy=0;
						//log_debug("From %d,%d to %d,%d", _xpointer, _ypointer, newx, newy);
						notify_mouse_moved(newx, newy);
						break;
					}
					case gnash::key::DOWN:
					{
						int newx = _xpointer;
						int newy = _ypointer+step;
						//log_debug("(theoretically) From %d,%d to %d,%d (step %d)", _xpointer, _ypointer, newx, newy, step);
						if ( newy >= _height ) newy = _height-1;
						//log_debug("From %d,%d to %d,%d", _xpointer, _ypointer, newx, newy);
						notify_mouse_moved(newx, newy);
						break;
					}
					case gnash::key::LEFT:
					{
						int newx = _xpointer-step;
						int newy = _ypointer;
						//log_debug("(theoretically) From %d,%d to %d,%d (step %d)", _xpointer, _ypointer, newx, newy, step);
						if ( newx < 0 ) newx = 0;
						//log_debug("From %d,%d to %d,%d", _xpointer, _ypointer, newx, newy);
						notify_mouse_moved(newx, newy);
						break;
					}
					case gnash::key::RIGHT:
					{
						int newy = _ypointer;
						int newx = _xpointer+step;
						//log_debug("(theoretically) From %d,%d to %d,%d (step %d)", _xpointer, _ypointer, newx, newy, step);
						if ( newx >= _width ) newx = _width-1;
						//log_debug("From %d,%d to %d,%d", _xpointer, _ypointer, newx, newy);
						notify_mouse_moved(newx, newy);
						break;
					}
					default:
						break;
				}
			}
#endif // ENABLE_KEYBOARD_MOUSE_MOVEMENTS
		}
	}

    if ( ! _started ) return;

    if ( _stopped ) return;

	if ( m->notify_key_event(k, pressed) )
	{
		// any action triggered by the
		// event required screen refresh
		display(m);
	}

}

bool
Gui::display(movie_root* m)
{
    assert(m == _stage); // why taking this arg ??

    assert(_started);

	InvalidatedRanges changed_ranges;
	bool redraw_flag;

	// Should the frame be rendered completely, even if it did not change?
#ifdef FORCE_REDRAW
  redraw_flag = true;
#else	
	redraw_flag = _redraw_flag || want_redraw();
#endif	
	
	// reset class member if we do a redraw now
	if (redraw_flag) _redraw_flag=false;

	// Find out the surrounding frame of all characters which
	// have been updated. This just checks what region of the stage has changed
	// due to ActionScript code, the timeline or user events. The GUI can still
	// choose to render a different part of the stage. 
	//
	if (!redraw_flag) {
		
		// choose snapping ranges factor 
		changed_ranges.setSnapFactor(1.3f);  
			
		// Use multi ranges only when GUI/Renderer supports it
		// (Useless CPU overhead, otherwise)
		changed_ranges.setSingleMode(!want_multiple_regions());

		// scan through all sprites to compute invalidated bounds  
		m->add_invalidated_bounds(changed_ranges, false);
		
		// grow ranges by a 2 pixels to avoid anti-aliasing issues		
		changed_ranges.growBy(40.0f / _xscale);
		
		// optimize ranges
		changed_ranges.combine_ranges();
		
	}

	if (redraw_flag)     // TODO: Remove this and want_redraw to avoid confusion!?
	{
		changed_ranges.setWorld();
	}
	
	// DEBUG ONLY:
  // This is a good place to inspect the invalidated bounds state. Enable
  // the following block (and parts of it) if you need to. 
#if 0
  {
    // This may print a huge amount of information, but is useful to analyze
    // the (visible) object structure of the movie and the flags of the
    // characters. For example, a characters should have set the 
    // m_child_invalidated flag if at least one of it's childs has the
    // invalidated flag set.
    log_debug("DUMPING CHARACTER TREE"); 
    m->dump_character_tree();
    
    // less verbose, and often necessary: see the exact coordinates of the
    // invalidated bounds (mainly to see if it's NULL or something else).	
    std::cout << "Calculated changed ranges: " << changed_ranges << "\n";
  }
#endif
  
	// Avoid drawing of stopped movies
	if ( ! changed_ranges.isNull() ) // use 'else'?
	{
		// Tell the GUI(!) that we only need to update this
		// region. Note the GUI can do whatever it wants with
		// this information. It may simply ignore the bounds
		// (which will normally lead into a complete redraw),
		// or it may extend or shrink the bounds as it likes. So,
		// by calling set_invalidated_bounds we have no guarantee
		// that only this part of the stage is rendered again.
#ifdef REGION_UPDATES_DEBUGGING_FULL_REDRAW
		// redraw the full screen so that only the
		// *new* invalidated region is visible
		// (helps debugging)
		InvalidatedRanges world_ranges;
		world_ranges.setWorld();
		setInvalidatedRegions(world_ranges);
#else
		setInvalidatedRegions(changed_ranges);
#endif

		// TODO: should this be called even if we're late ?
		beforeRendering();

		// Render the frame, if not late.
		// It's up to the GUI/renderer combination
		// to do any clipping, if desired.     
		m->display();
  
		// show invalidated region using a red rectangle
		// (Flash debug style)
		IF_DEBUG_REGION_UPDATES (
		if ( ! changed_ranges.isWorld() )
		{
		
			for (size_t rno = 0; rno < changed_ranges.size(); rno++) {
			
				geometry::Range2d<float> bounds = changed_ranges.getRange(rno);

				point corners[4];
				float xmin = bounds.getMinX();
				float xmax = bounds.getMaxX();
				float ymin = bounds.getMinY();
				float ymax = bounds.getMaxY();
				
				corners[0].x = xmin;
				corners[0].y = ymin;
				corners[1].x = xmax;
				corners[1].y = ymin;
				corners[2].x = xmax;
				corners[2].y = ymax;
				corners[3].x = xmin;
				corners[3].y = ymax;
				matrix no_transform;
				gnash::render::draw_poly(corners, 4,
					rgba(0,0,0,0), rgba(255,0,0,255), no_transform, false);
					
			}
		}
		);

		// show frame on screen
		renderBuffer();
   	
	};
  
	return true;
}

void
Gui::play()
{
    if ( ! _stopped ) return;

    _stopped = false;
    if ( ! _started ) start();

    playHook ();
}

void
Gui::stop()
{
    if ( _stopped ) return;
    if ( isFullscreen() ) unsetFullscreen();

    _stopped = true;

    stopHook();
}

void
Gui::pause()
{
    if ( _stopped )
    {
        play();
    }
    else _stopped = true;
}

void
Gui::start()
{
    assert ( ! _started );
    if ( _stopped )
    {
        log_debug("Gui is in stop mode, won't start application");
        return;
    }

    std::auto_ptr<movie_instance> mr ( _movieDef->create_movie_instance() );
    mr->setVariables(_flashVars);

    _stage->setRootMovie( mr.release() ); // will construct the instance
    resize_view(_width, _height); // to properly update stageMatrix if scaling is given 

    bool background = true; // ??
    _stage->set_background_alpha(background ? 1.0f : 0.05f);

    _started = true;
}

bool
Gui::advanceMovie()
{
    static unsigned long advances = 0;

	if ( isStopped() ) return true;

    if ( ! _started ) start();
  


#ifdef SKIP_RENDERING_IF_LATE
	WallClockTimer advanceTimer;
#endif // SKIP_RENDERING_IF_LATE

	gnash::movie_root* m = _stage;
	
#ifdef GNASH_FPS_DEBUG
	fpsCounterTick(); // will be a no-op if fps_timer_interval is zero
#endif

// Define REVIEW_ALL_FRAMES to have *all* frames
// consequencially displaied. Useful for debugging.
//#define REVIEW_ALL_FRAMES 1

#ifndef REVIEW_ALL_FRAMES
	// Advance movie by one frame
	m->advance();
#else
	size_t cur_frame = m->getRootMovie()->get_current_frame();
	size_t tot_frames = m->getRootMovie()->get_frame_count();
	m->advance();
	m->get_movie_definition()->ensure_frame_loaded(tot_frames);
	m->goto_frame(cur_frame+1);
    	m->set_play_state(gnash::sprite_instance::PLAY);
	log_debug(_("Frame %d"), m->get_current_frame());
#endif


#ifdef SKIP_RENDERING_IF_LATE

	boost::uint32_t advanceTime = advanceTimer.elapsed(); // in milliseconds !

	boost::uint32_t timeSlot = _interval; // milliseconds between advance calls 

	if ( advanceTime+gui->estimatedDisplayTime < timeSlot )
	{
		advanceTimer.restart();
		display(m);
		boost::uint32_t displayTime = advanceTimer.elapsed();

		if ( displayTime > estimatedDisplayTime)
		{
			//log_debug("Display took %6.6g seconds over %6.6g available for each frame", displayTime, timeSlot);

			// Don't update estimatedDisplayTime if it's bigger then timeSlot*0.8
			if (  displayTime < timeSlot*0.8 )
			{
				// TODO: check for absurdly high values, like we can't set
				//       estimatedDisplayTime to a value higher then FPS, or
				//       we'll simply never display...
				estimatedDisplayTime = displayTime;
			}
		}
	}
	else
	{
		log_debug("We're unable to keep up with FPS speed: "
			"advanceTime was %u + estimatedDisplayTime (%u) "
			"== %u, over a timeSlot of %u",
			advanceTime, estimatedDisplayTime,
			advanceTime+estimatedDisplayTime, timeSlot);
		// TODO: increment a counter, we don't want to skip too many frames
	}
#else // ndef SKIP_RENDERING_IF_LATE

	display(m);

#endif // ndef SKIP_RENDERING_IF_LATE
	
	if ( ! loops() )
	{
		size_t curframe = m->get_current_frame(); // can be 0 on malformed SWF
		gnash::sprite_instance* si = m->getRootMovie();
		if (curframe + 1 >= si->get_frame_count())
		{
			quit(); 
		}
	}

    /// Quit if we've reached the advance limit.
    if (_maxAdvances && (advances++ > _maxAdvances))
    {
        quit();
    }

	return true;
}

void
Gui::setCursor(gnash_cursor_type /*newcursor*/)
{
}

bool
Gui::want_redraw()
{
    return false;
}

bool
Gui::loops()
{
    return _loop;
}

void
Gui::setInvalidatedRegion(const rect& /*bounds*/)
{
}

void
Gui::setInvalidatedRegions(const InvalidatedRanges& ranges)
{
	// fallback to single regions
	geometry::Range2d<float> full = ranges.getFullArea();
	
	rect bounds;
	
	if (full.isFinite())
		bounds = rect(full.getMinX(), full.getMinY(), full.getMaxX(), full.getMaxY());
	else
	if (full.isWorld())
		bounds.set_world();
	
	setInvalidatedRegion(bounds);
}

#ifdef USE_SWFTREE

std::auto_ptr<Gui::InfoTree>
Gui::getMovieInfo() const
{
    std::auto_ptr<InfoTree> tr;

    if ( ! VM::isInitialized() )
    {
        return tr;
    }

    tr.reset(new InfoTree());

    // Top nodes for the tree:
    // 1. VM information
    // 2. "Stage" information
    // 3. ...

    InfoTree::iterator topIter = tr->begin();
    InfoTree::iterator firstLevelIter;

    VM& vm = VM::get();

    std::ostringstream os;

    //
    /// VM top level
    //
    os << "SWF " << vm.getSWFVersion();
    topIter = tr->insert(topIter, StringPair("VM version", os.str()));


    movie_root& stage = vm.getRoot();
    stage.getMovieInfo(*tr, topIter);

    //
    /// Mouse entities
    //
    topIter = tr->insert(topIter, StringPair("Mouse Entities", ""));

    const character* ch;
    ch = stage.getActiveEntityUnderPointer();
    if ( ch )
    {
	    std::stringstream ss;
	    ss << ch->getTarget() << " (" + typeName(*ch)
            << " - id:" << ch->get_id()
            << " - depth:" << ch->get_depth()
            << " - useHandCursor:" << ch->allowHandCursor()
            << ")";
    	firstLevelIter = tr->append_child(topIter, StringPair("Active entity under mouse pointer", ss.str()));
    }

    ch = stage.getEntityUnderPointer();
    if ( ch )
    {
	    std::stringstream ss;
	    ss << ch->getTarget() << " (" + typeName(*ch) 
               << " - id:" << ch->get_id() << " - depth:" << ch->get_depth()
               << ")";
	firstLevelIter = tr->append_child(topIter, StringPair("Topmost entity under mouse pointer", ss.str()));
    }

    ch = stage.getDraggingCharacter();
    if ( ch ) 
    {
	    std::stringstream ss;
	    ss << ch->getTarget() << " (" + typeName(*ch) 
               << " - id:" << ch->get_id()
               << " - depth:" << ch->get_depth() << ")";
    	firstLevelIter = tr->append_child(topIter, StringPair("Dragging character: ", ss.str()));
    }

    //
    /// GC row
    //
    topIter = tr->insert(topIter, StringPair("GC Statistics", ""));
    GC::CollectablesCount cc;
    GC::get().countCollectables(cc);
    
    const std::string lbl = "GC managed ";
    for (GC::CollectablesCount::iterator i=cc.begin(), e=cc.end(); i!=e; ++i)
    {
        const std::string& typ = i->first;
        std::ostringstream ss;
        ss << i->second;
        firstLevelIter = tr->append_child(topIter,
                            StringPair(lbl + typ, ss.str()));
    }

    tr->sort(firstLevelIter.begin(), firstLevelIter.end());

    return tr;
}

#endif

#ifdef GNASH_FPS_DEBUG
void 
Gui::fpsCounterTick()
{

  // increment this *before* the early return so that
  // frame count on exit is still valid
  ++fps_counter_total;

  if ( ! fps_timer_interval )
  {
	  return;
  }

  boost::uint64_t current_timer = clocktime::getTicks();

  // TODO: keep fps_timer_interval in milliseconds to avoid the multiplication
  //       at each fpsCounterTick call...
  boost::uint64_t interval_ms = (boost::uint64_t)(fps_timer_interval * 1000.0);

  if (fps_counter_total==1) {
    fps_timer = current_timer;
    fps_start_timer = current_timer;
  }
  
  ++fps_counter;
  
  if (current_timer - fps_timer >= interval_ms) {
  
    float secs = (current_timer - fps_timer) / 1000.0;
    float secs_total = (current_timer - fps_start_timer)/1000.0;
        
    float rate = fps_counter/secs;
    
    if (secs > 10000000) {
      // the timers are unsigned, so when the clock runs "backwards" it leads
      // to a very high difference value. In theory, this should never happen
      // with ticks, but it does on my machine (which may have a hw problem?).
      std::cerr << "Time glitch detected, need to restart FPS counters, sorry..." << std::endl;
      
      fps_timer = current_timer;
      fps_start_timer = current_timer;
      fps_counter_total = 0;
      fps_counter = 0;
      return;
    } 
     
    // first FPS message?
    if (fps_timer == fps_start_timer) {     // they're ints, so we can compare
      fps_rate_min = rate;
      fps_rate_max = rate; 
    } else {
      fps_rate_min = std::max<float>(fps_rate_min, rate);
      fps_rate_max = std::max<float>(fps_rate_max, rate);
    }
    
    float avg = fps_counter_total / secs_total; 
  
    //log_debug("Effective frame rate: %0.2f fps", (float)(fps_counter/secs));
    std::cerr << boost::format("Effective frame rate: %0.2f fps "
                               "(min %0.2f, avg %0.2f, max %0.2f, "
                               "%u frames in %0.1f secs total)") % rate %
                               fps_rate_min % avg % fps_rate_max %
                               fps_counter_total % secs_total << std::endl;
      
    fps_counter = 0;
    fps_timer = current_timer;
    
  }
   
}
#endif

void
Gui::addFlashVars(Gui::VariableMap& from)
{
    for (VariableMap::iterator i=from.begin(), ie=from.end(); i!=ie; ++i)
    {
        _flashVars[i->first] = i->second;
    }
}

void
Gui::setMovieDefinition(movie_definition* md)
{
    assert(!_movieDef);
    _movieDef = md;
}

void
Gui::setStage(movie_root* stage)
{
    assert(stage);
    assert(!_stage);
    _stage = stage;
}

// end of namespace
}
