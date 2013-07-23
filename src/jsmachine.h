/* jscam - C++ CAD/CAM driver library.
 * Copyright (C) 2013  Nicholas Gill
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * jsmachine.h
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#ifndef JSMACHINE_H_
#define JSMACHINE_H_
#include <v8.h>

namespace jsmachine
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

m.exact_path();
m.exact_stop();
m.path_blend(p, q);

m.motion = absoloute/incremental;
m.arc_motion = absoloute/incremental;
m.units = metric/imperial;
m.plane = XY/XZ...;
m.feed_rate_mode = ...;
m.feed_rate = 30.5;
m.spindle_on(speed, direction);
m.spindle_off();
var x = m.spindle;

m.load_tool(id)
m.tool_change(id);
var x = m.tool;

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
