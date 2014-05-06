"use strict";
load("gcode.js");

/* Declcam Part compiler
 * called as jscam partc.js the_part.part
 * */

var part = JSON.parse(read(args[1]));
var machine_library;
var tool_library;

var len = args.length;
for (var i = 0; i < len; ++i) {
    var arg = args[i];
    if(arg == "--machines") {
        machine_library = args[++i];
    } else if(arg == "--tools") {
        tool_library = args[++i];
    }
}

function build_machine(decl, tool_table) {
    var m = decl;
    /* TODO resolve tools in tool table and map to tools
     * in machine object. */

    var machine = new Machine(m);
    return machine;
}

var test =
{
    machine: "Sherline 4-Axis Mill",
	type: "mill",
    units: "metric",
    axes: "XYZA",
    tools: {},
	spindle: [ {rpm: [70, 2800], nm: [3.1776, 0.0706]} ],
	feed_rate: {z: 50, max: 300},
	rapid_rate: {z: 50, max: 300}
};

var m = build_machine(test);
print("Compiling part: " + part.part);

m.begin_block("prelude");
m.comment("Part       : " + part.part);
m.comment("Description: " + part.description);
m.comment("Machine    : " + part.machine);
m.comment("Material   : " + part.material);
m.end_block();

var gc = m.generate();
print(GCODE.generate(gc));


