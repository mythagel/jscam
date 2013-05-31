# TODO List #

### High Level ###
 * Include paths
    - For library files
 * cxxcam bindings:
    - Corordinate systems
    - State push/pop
    - nef::polyhedron_t
 * JS function library
    - Convert model to Three.js format for display
    - Backplot (Preferably 3d projection using 2d html5 canvas)
 * Machining operations
    - Profile
    - Pocket
    - Drill
    - Facing
    - Slots
    - Lathe (Given a curve)
    - ???
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
 * NodeJS module (refactor so same binding code can be used)
    - http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
    - Portability of scripts has to be considered. It is desired that scripts be self-contained so they can easily be regenerated on different systems.

### Code ###
