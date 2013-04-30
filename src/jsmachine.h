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

"machine":
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

init(machine, "LinuxCNC")

exact_path()
exact_stop()
path_blend()
path_blend(p)
path_blend(p, q)

motion(absoloute/incremental)
arc_motion(absoloute/incremental)

units(metric/imperial)

plane(XY/XZ...)

feed_rate_mode(...)

feed_rate(30.5);

spindle_on(speed, direction)
spindle_off()

load_tool(id)
tool_change(id)

begin_block(name)
end_block(restore)

optional_pause(comment)

rapid(xyzabc)
rapid( {"x": "5"}, {"y": "50"} )

linear(xyzabc)

arc(...)

plunge(...)

 */
void bind(v8::Handle<v8::ObjectTemplate> global);

}

#endif /* JSMACHINE_H_ */
