"use strict";

load("mill.js");

load("../lib/spiral.js");
load("../lib/gcode.js");

var m = new Machine(mill);

/*
pocket.js
=========

*/

function polygon(sides, size, center, depth)
{
	m.begin_block("polygon od " + size + " @ (" + center.x + "," + center.y + ")");
	
	var tool = m.tool;
	//tool.mill_diameter
	// TODO spiral in pocket by mill width steps
	
	m.rapid ({x: center.x + size * Math.cos(0), y: center.y + size * Math.sin(0)});         
	m.linear({z: depth});

	for (var side = 1; side <= sides; ++side)
	{
		var x = center.x + size * Math.cos(side * 2 * Math.PI / sides);
		var y = center.y + size * Math.sin(side * 2 * Math.PI / sides);
		m.linear({x: x, y: y});
	}
	m.end_block(Machine.BlockRestore.RestoreAll);
}

m.begin_block("polygon test");

m.tool_change(1);
m.feed_rate = 100;
m.spindle_on(100);

m.begin_block("polygon grid");
for(var i = 0; i < 10; ++i)
{
	polygon(6, 10, {x: (20*i) + 5, y: 0}, -1);
}
m.end_block();

m.end_block(Machine.BlockRestore.RestoreAll);


m.begin_block("spiral test");
m.tool_change(1);
m.feed_rate = 100;
m.spindle_on(100);
spiral(m, {x: 0, y: 0}, 72, 10, 0, 3, 0);
m.end_block(Machine.BlockRestore.RestoreAll);

print(generate_gcode(m.generate()));
//write("test.ngc", generate(m));

/*
 * Format: https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3.1
 */
print(JSON.stringify(m.generate_model()));

