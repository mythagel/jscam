/*
 * jsmachine.h
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#ifndef JSMACHINE_H_
#define JSMACHINE_H_
#include <v8.h>

namespace jscam
{

/*
API Sketch

"mill":
{
	"type": "mill/lathe",
	"tools":
	[
		{
			"name": "tool_name",
			"id": "int",
			"type": "mill/lathe",

			"center_cutting": "true/false",
			"flutes": "int",
			"flute_length": "",
			"cutting_length": "",
			"mill_diameter": "",
			"shank_diameter": "",
			"core_diameter": "",
			"length": ""
		},
		{...}
	],
	"spindle": [ "0-100", 300, "500-1000", 3000]
}

var m = new Machine(mill, "LinuxCNC");

m.trajectory = exact_path / exact_stop / {blend: []} / {blend: [p]} / {blend: [p, q]};
m.motion = absoloute/incremental;
m.arc_motion = absoloute/incremental;
m.units = metric/imperial;
m.plane = XY/XZ...;
m.feed_rate_mode = ...;
m.feed_rate = 30.5;
m.spindle = { dir: stop/clockwise/counterclockwise, speed: x };

m.load_tool(id)
m.tool = id;

m.begin_block(name);
m.end_block(restore);

m.optional_pause(comment);
m.rapid(xyzabc)
m.rapid({x: 5, y: 50})
m.linear(xyzabc)
m.arc(...)
m.plunge(...)
m.generate();

 */
void bind(v8::Handle<v8::Object> global);

}

#endif /* JSMACHINE_H_ */
