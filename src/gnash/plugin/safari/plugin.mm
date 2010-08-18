// plugin.cpp:  Windows "win32" flash player Mozilla plugin, for Gnash.
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

//#include <cstdlib>

#define FLASH_MAJOR_VERSION "9"
#define FLASH_MINOR_VERSION "0"
#define FLASH_REV_NUMBER "82"

#define MIME_TYPES_HANDLED "application/x-shockwave-flash"
// The name must be this value to get flash movies that check the
// plugin version to load.
#define PLUGIN_NAME "Shockwave Flash"
#define MIME_TYPES_DESCRIPTION MIME_TYPES_HANDLED":swf:"PLUGIN_NAME

// Some javascript plugin detectors use the description
// to decide the flash version to display. They expect the
// form (major version).(minor version) r(revision).
// e.g. "8.0 r99."
#ifndef FLASH_MAJOR_VERSION
#define FLASH_MAJOR_VERSION DEFAULT_FLASH_MAJOR_VERSION
#endif
#ifndef FLASH_MINOR_VERSION
#define FLASH_MINOR_VERSION DEFAULT_FLASH_MINOR_VERSION
#endif
#ifndef FLASH_REV_NUMBER
#define FLASH_REV_NUMBER DEFAULT_FLASH_REV_NUMBER
#endif
#define FLASH_VERSION FLASH_MAJOR_VERSION"."\
    FLASH_MINOR_VERSION" r"FLASH_REV_NUMBER"."

#define PLUGIN_DESCRIPTION \
  "Shockwave Flash "FLASH_VERSION" Gnash "VERSION", the GNU Flash Player. \
  Copyright &copy; 2006, 2007, 2008 \
  <a href=\"http://www.fsf.org\">Free Software Foundation</a>, Inc.<br> \
  Gnash comes with NO WARRANTY, to the extent permitted by law. \
  You may redistribute copies of Gnash under the terms of the \
  <a href=\"http://wwinitWithArgumentsw.gnu.org/licenses/gpl.html\">GNU General Public \
  License</a>. For more information about Gnash, see <a \
  href=\"http://www.gnu.org/software/gnash/\"> \
  http://www.gnu.org/software/gnash</a>. \
  Compatible Shockwave Flash "FLASH_VERSION

#include "plugin.h"

static int initCount = 0;



#define DBG(x, ...) __DBG(x, ## __VA_ARGS__)
inline void
__DBG(const char *fmt, ...)
{
    char buf[1024];
    va_list ap;
    
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);
    fprintf(stderr, buf);
}

@class DOMElement;

@interface GnashView (Internal)
- (id)_initWithArguments:(NSDictionary *)arguments;
@end

@implementation GnashView

@synthesize container;

-(void)drawRect
{
//	[[UIColor redColor] set];
//	
//	UIRectFill(rect);

//    fprintf(stderr, "Draw called");
//	
//	[[UIColor whiteColor] set];
//	
//	[@"Flash goes here..." drawInRect:[self bounds] withFont:[UIFont boldSystemFontOfSize:16.0]];

   //glClearColor ( 1.0f, 0.0f, 0.0f, 0.0f );

  // glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);

//   // Set the viewport
//   glViewport ( 0, 0, 100.0f, 100.0f );
//   
//   // Clear the color buffer
//   glClear ( GL_COLOR_BUFFER_BIT );

//   glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
//	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
       
    // Register this plugin as listener for FsCommands from the core
    // (movie_root)
    //root.registerFSCommandCallback(FSCommand_callback);
    
    // Register a static function to handle ActionScript events such
    // as Mouse.hide, Stage.align etc.
    // root.registerEventCallback(&staticEventHandlingFunction);

     
    

//    for (;;) 
    //{
  //       DBG("Inside main thread loop, %X.\n", root);

        size_t cur_frame = mi->get_current_frame();
        //DBG("Got current frame number: %d.\n", cur_frame);
        size_t tot_frames = mi->get_frame_count();
        // DBG("Got total frame count: %d.\n", tot_frames);

//         DBG("Advancing one frame.\n");
        mi->advance();
  //       DBG("Going to next frame.\n");
        mi->goto_frame(cur_frame + 1);
    //     DBG("Ensuring frame is loaded.\n");
        mi->get_movie_definition()->ensure_frame_loaded(tot_frames);
      //   DBG("Setting play state to PLAY.\n");
        mi->set_play_state(gnash::sprite_instance::PLAY);

//        if (old_mouse_x != mouse_x || old_mouse_y != mouse_y) {
//            old_mouse_x = mouse_x;
//            old_mouse_y = mouse_y;
//            root.notify_mouse_moved(mouse_x, mouse_y);
//        }
//        if (old_mouse_buttons != mouse_buttons) {
//            old_mouse_buttons = mouse_buttons;
//            int mask = 1;
//            root.notify_mouse_clicked(mouse_buttons > 0, mask);
//        }

        root->display();

       sdlaggglue.render();

        //DBG("Sleeping\n");
//    }

done:

    //DBG("Clean up Gnash.\n");

    /*
     * N.B.  As per server/impl.cpp:clear(), all of Gnash's threads aren't
     * guaranteed to be terminated by this, yet.  Therefore, when Firefox
     * unloads npgnash.dll after calling NS_PluginShutdown(), and there are
     * still Gnash threads running, they will try and access memory that was
     * freed as part of the unloading of npgnash.dll, resulting in a process
     * abend.
     */

    //CGRect rectb = CGRectMake(0, 0, 100, 100);
    [self performSelector:@selector(drawRect) withObject:nil afterDelay:1.0f/movie_fps];
    

    //DBG("nsPluginInstance::threadMain exiting\n");

   
}

- (NSString*) flashvarWithName: (NSString*) argName
{
    return [ _flashVars objectForKey: argName ];
}

- (id)initWithFrame:(CGRect)aRect
{

    _width = 550;
    _height = 400;

    fprintf(stderr, "Init with frame %d %d\n", _width, _height);
    
    aRect =  CGRectMake(0, 0, _width, _height);

    fprintf(stderr, "Init with frame called, %.f %.f %.f %.f\n", aRect.origin.x, aRect.origin.y, aRect.size.width, aRect.size.height);

    if (aRect.size.width == 0)
        fprintf(stderr, "width is zero\n");

    self.frame = aRect;

     return [super initWithFrame: aRect \
									retainBacking: YES \
									rBits: 8 \
									gBits: 8 \
									bBits: 8 \
									aBits: 8 \
									depthBits: 0];
//    return [super initWithFrame: aRect];
}

// WebPlugInViewFactory protocol
// The principal class of the plug-in bundle must implement this protocol.

+ (UIView *)plugInViewWithArguments:(NSDictionary *)newArguments
{
	
	//NSLog(@"IPAD WEB VIEW ARGS = %@", [newArguments description]);

	NSDictionary *pluginDict = [newArguments objectForKey:@"WebPlugInAttributesKey"];

    
	
//    _flashURL =[NSString stringWithString:[pluginDict objectForKey:@"src"]];

    _width = [[pluginDict objectForKey:@"width"] intValue];
    _height = [[pluginDict objectForKey:@"height"] intValue];


    fprintf(stderr, "Plugin started, %s ,%d %d\n", [[pluginDict objectForKey:@"src"] cString], _width, _height);

    NSArray *keys;
    int i, count;
    id key, value;

    keys = [newArguments allKeys];
    count = [keys count];

    NSLog(@"%@", newArguments);

//    for (i = 0; i < count; i++)
//    {
//        key = [keys objectAtIndex: i];
//        value = [newArguments objectForKey: key];
//        NSString *stringRep = [NSString stringWithFormat:@"%@",key];
//        NSString *stringRep1 = [NSString stringWithFormat:@"%@",value];
//        fprintf(stderr,  "Key: %s for value: %s\n", [stringRep cString], [stringRep1 cString]);
//    }
	
    return [[[self alloc] _initWithArguments:newArguments] autorelease];
}

// WebPlugIn informal protocol

- (void)webPlugInInitialize
{
    // This method will be only called once per instance of the plug-in object, and will be called
    // before any other methods in the WebPlugIn protocol.
    // You are not required to implement this method.  It may safely be removed.
	
	fprintf(stderr, "MobileSafariPlugin demo plugin loaded");

    
    

}

- (void)webPlugInStart
{
    // The plug-in usually begins drawing, playing sounds and/or animation in this method.
    // You are not required to implement this method.  It may safely be removed.
    md = 0;

    DBG("nsPluginInstance::threadMain started\n");

    CGRect aRect = CGRectMake(0, 0, _width, _height);

    self.frame = aRect;
//    DBG("URL: %s\n", _url.c_str());

    // Initialize Gnash core library.
    
    if (initCount == 0)
    {
        gnash::gnashInit();
        initCount++;
        
    }
    else 
        {
        initCount++;
        return;
    }
    DBG("Gnash core initialized.\n");
 
    // Init logfile.
    gnash::RcInitFile& rcinit = gnash::RcInitFile::getDefaultInstance();
//    std::string logfilename = std::string(std::getenv("TEMP")) +
//        std::string("\\npgnash.log");
//    rcinit.setDebugLog(logfilename);
//    gnash::LogFile& dbglogfile = gnash::LogFile::getDefaultInstance();
//    dbglogfile.setWriteDisk(true);
//    dbglogfile.setVerbosity(GNASH_DEBUG_LEVEL);
//    DBG("Gnash logging initialized: %s\n", logfilename.c_str());

    // Init sound.
    _sound_handler.reset(gnash::media::create_sound_handler_sdl());
    gnash::set_sound_handler(_sound_handler.get());
    DBG("Gnash sound initialized.\n");

    // Init GUI.
    int old_mouse_x = 0, old_mouse_y = 0, old_mouse_buttons = 0;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    atexit(SDL_Quit);

    sdlaggglue.createRenderHandler(32);

    sdlaggglue.prepDrawingArea(_width, _height, 0, self.window, self);

    fprintf(stderr, "Setting the render handler\n");

    gnash::set_render_handler(sdlaggglue._agg_renderer);

    fprintf(stderr, "Parsing the URL %s\n", [_flashURL cString]);

    gnash::URL url([_flashURL cString]);

//        

//    VariableMap vars;
//    gnash::URL::parse_querystring(gnash::URL(_url).querystring(), vars);

    VariableMap vars;
    gnash::URL::parse_querystring(url.querystring(), vars);

    fprintf(stderr, "Saving vars\n");
    for (VariableMap::iterator i = vars.begin(), ie = vars.end(); i != ie; i++) {
        _flashVars2[i->first] = i->second;
    }

    fprintf(stderr, "create_library_movie\n");

    gnash::set_base_url(url);

    try {
        md = gnash::create_library_movie(url, [_flashURL cString], false);
    } catch (const gnash::GnashException& err) {
        md = NULL;
    }
    if (!md) {
        /*
         * N.B. Can't use the goto here, as C++ complains about "jump to
         * label 'done' from here crosses initialization of ..." a bunch
         * of things.  Sigh.  So, instead, I duplicate the cleanup code
         * here.  TODO: Remove this duplication.
         */
        // goto done;

        DBG("Clean up Gnash.\n");
        gnash::clear();

        DBG("nsPluginInstance::threadMain exiting\n");
        return;
    }
    DBG("Movie created: %s\n", [_flashURL cString]);

    int movie_width = static_cast<int>(md->get_width_pixels());
    int movie_height = static_cast<int>(md->get_height_pixels());
    movie_fps = md->get_frame_rate();
    DBG("Movie dimensions: %ux%u (%.2f fps)\n",
            movie_width, movie_height, movie_fps);

    gnash::SystemClock clock; // use system clock here...
    gnash::movie_root& root2 = gnash::VM::init(*md, clock).getRoot();
    DBG("Gnash VM initialized.\n");

    root = &root2;
    
    // Register this plugin as listener for FsCommands from the core
    // (movie_root)
    //root.registerFSCommandCallback(FSCommand_callback);
    
    // Register a static function to handle ActionScript events such
    // as Mouse.hide, Stage.align etc.
    // root.registerEventCallback(&staticEventHandlingFunction);

    md->completeLoad();
    DBG("Movie loaded.\n");

    std::auto_ptr<gnash::movie_instance> mr(md->create_movie_instance());
    mr->setVariables(_flashVars2);
    root->setRootMovie(mr.release());
    root->set_display_viewport(0, 0, _width, _height);
    root->set_background_alpha(1.0f);
    mi = root->getRootMovie();
    DBG("Movie instance created.\n");

    DBG("Inside main thread loop, %X.\n", root);

        size_t cur_frame = mi->get_current_frame();
        DBG("Got current frame number: %d.\n", cur_frame);
        size_t tot_frames = mi->get_frame_count();
         DBG("Got total frame count: %d.\n", tot_frames);

         DBG("Advancing one frame.\n");
        root->advance();
         DBG("Going to next frame.\n");
        root->goto_frame(cur_frame + 1);
         DBG("Ensuring frame is loaded.\n");
        root->get_movie_definition()->ensure_frame_loaded(tot_frames);
         DBG("Setting play state to PLAY.\n");
        root->set_play_state(gnash::sprite_instance::PLAY);

    CGRect rect = CGRectMake(0, 0, 100, 100);
    [self performSelector:@selector(drawRect) withObject:nil afterDelay:1.0f/movie_fps];

}

- (void)webPlugInStop
{
    // The plug-in normally stop animations/sounds in this method.
    // You are not required to implement this method.  It may safely be removed.
}

- (void)webPlugInDestroy
{
    // Perform cleanup and prepare to be deallocated.
    // You are not required to implement this method.  It may safely be removed.
    gnash::clear();
}

- (void)webPlugInSetIsSelected:(BOOL)isSelected
{
    // This is typically used to allow the plug-in to alter its appearance when selected.
    // You are not required to implement this method.  It may safely be removed.
}

- (id)objectForWebScript
{
    // Returns the object that exposes the plug-in's interface.  The class of this object can implement
    // methods from the WebScripting informal protocol.
    // You are not required to implement this method.  It may safely be removed.
    return self;
}

#define CTFForEachObject( Type, varName, container ) \
NSEnumerator* feoEnum_##__LINE__ = [ container objectEnumerator ]; \
Type* varName; \
while( ( varName = [ feoEnum_##__LINE__ nextObject ] ) )

- (NSDictionary*) _flashVarDictionary: (NSString*) flashvarString
{
    NSMutableDictionary* flashVarsDictionary = [ NSMutableDictionary dictionary ];
    
    NSArray* args = [ flashvarString componentsSeparatedByString: @"&" ];
    
    CTFForEachObject( NSString, oneArg, args ) {
        NSRange sepRange = [ oneArg rangeOfString: @"=" ];
        if( sepRange.location != NSNotFound ) {
            NSString* key = [ oneArg substringToIndex: sepRange.location ];
            NSString* val = [ oneArg substringFromIndex: NSMaxRange( sepRange ) ];
            
            [ flashVarsDictionary setObject: val forKey: key ];
        }
    }
    
    return flashVarsDictionary;
}
@end

@implementation GnashView (Internal)

- (id)_initWithArguments:(NSDictionary *)newArguments
{
    fprintf(stderr, "Init called\n");

  	NSDictionary *pluginDict = [newArguments objectForKey:@"WebPlugInAttributesKey"];

    _flashURL = [[NSString alloc] initWithString:[pluginDict objectForKey:@"src"]];

    NSURL *baseURL = [newArguments objectForKey:@"WebPlugInBaseURLKey"];
    fprintf(stderr, "baseURL %s", [[baseURL absoluteString ] cString]);
    NSURL *URL = [NSURL URLWithString:_flashURL relativeToURL:baseURL];
    _flashURL = [[NSString alloc] initWithString:[URL absoluteString]];

//[ stringWithString:[pluginDict objectForKey:@"src"]];

    fprintf(stderr, "Init called2, %s,%d %d\n", [_flashURL cString], _width, _height);



    if (!(self = [super init]))
        return nil;
	
	return self;
	
	/* Specific C-T-F plugin code has been broken in final 3.2 SDK; will fix later */
	
	/*
	self.container = [newArguments objectForKey:@"WebPlugInContainingElementKey"];
	NSLog(@"FOUND FLASH = %@", [self.container description]);
	
	
	NSString* flashvars = [[newArguments objectForKey:@"WebPlugInAttributesKey"] objectForKey: @"flashvars" ];
	

	if( flashvars != nil )
		_flashVars = [ [ self _flashVarDictionary: flashvars ] retain ];
	
	
	DOMElement* newElement = (DOMElement*) [ [self container] cloneNode: NO ];
    
    [ self _convertElementForMP4: newElement ];
	
	[[[self container] parentNode] replaceChild:newElement oldChild:[self container]];
    [self setContainer:nil];

    
    return self;
	 */
}

@end

