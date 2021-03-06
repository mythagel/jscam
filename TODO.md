# TODO List #

### High Level ###
 * Include paths
    - For library files
 * cxxcam bindings:
    - Configuration
       - Feed rates
       - Rapid rates
    - Lathe tool
 * JS function library
    - Convert model to Three.js format for display
    - Backplot Viewer (Preferably 3d projection using 2d html5 canvas)
 * Machining operations
    - Profile
    - Pocket
    - Drill
    - Facing
    - Slots
    - Lathe (Given a curve)
    - ???
 * GCode Postprocessing
    - GCode Parser needed
       - Parses a gcode file into json object
    - Postprocessors
       - Rename axis
       - Translate rotational axes (i.e. rotational axis offsets)
       - Arcs to lines
       - Backplot
    - Modal code tracking
 * GCode Generation
    - Aligned comments within a gcode block
    - Document in comments relevant parameters (Stock dimensions, position, tools used, etc.)
 * One file is the complete part
    - With multiple operations underneath
    - Should be able to generate gcode for ops individually.
    - one file is NOT the complete object - only a single part.
 * LOGO / Turtle style interface for experimentation (implemented 100% in js)
 * Output image of toolpath (js or cxxcam)
 * Unchecked pure js Machine interface (for browser things)
    - Lower priority
    - Would allow unchecked conversion from js to nc code.
 * NodeJS module (refactor so same binding code can be used)
    - http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
    - Portability of scripts has to be considered. It is desired that scripts be self-contained so they can easily be regenerated on different systems.

### Code ###
 * Remove platform code
    - Replace with boost
    - Add additional js bindings from boost::fs
    - Need functions to enumerate path.
 * Bindings
    - Cleanup (e.g. Arc).
    - Generalise
       - Functions to convert js arrays and objects to axes etc.
 * Tests for V8 comparisions String::New vs String::NewSymbol and comparisions wrt, JSON::parse
 * Investigate issues with nested file inclusion

