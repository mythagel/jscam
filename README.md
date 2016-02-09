jscam
=====

js shell for [cxxcam](https://github.com/mythagel/cxxcam) driver

A procedural driver for GCode generation. Conceptually similar to a high level language for GCode generation. Use normal javascript looping and branching constructs to generate GCode.

See [lib/partc.js](https://github.com/mythagel/jscam/blob/master/lib/partc.js) as the primary intended interface. A higher level JSON based (declarative) language for describing parts and generating GCode.

See [rc-car](https://github.com/mythagel/rc-car) as the development example of parts declared in this method (_not_ real parts for an rc-car).

`jscam` is usable as a procedural gcode generator.
`partc` is not yet operational. Procedural code for machining operations is not yet developed and interfaces for model feedback from `cxxcam` for motion planning do not yet exist.

**DEPRECATED IN FAVOUR OF nc_tools**
