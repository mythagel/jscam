# TODO List #

### High Level ###
 * JS function library
 * Toolpath export
 * NodeJS module (refactor so same binding code can be used)
    - http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
    - Portability of scripts has to be considered. It is desired that scripts be self-contained so they can easily be regenerated on different systems.
 * Include paths
    - For library files
    - For user included files
    - At present only `cwd` is searched.
 * Unchecked pure js Machine interface (for browser things)
    - Lower priority
 * ~~Refine js api (some functions can be consolidated)~~
 * ~~Test harnesses and drivers to validate cxxcam functions~~
 * ~~Bind Tool loading from json (pending work in cxxcam)~~
 * ~~Bind functions to retrieve generated GCode~~
 * ~~WebGL export of model~~ (Interface exists)

### Code ###
 * Catch v8 exceptions and handle correct (currently just segfault in some cases of invalid input)
 * ~~Catch cxxcam exceptions and propagate as js exceptions~~
