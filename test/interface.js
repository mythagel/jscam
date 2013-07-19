"use strict";

function jprint(x)
{
	print(JSON.stringify(x));
}

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZ",
	tools: [
			{
			name: "3mm End Mill",
			id: 1,
			type: "mill",
			center_cutting: true,
			flutes: 4,
			flute_length: 25,
			cutting_length: 20,
			mill_diameter: 3,
			shank_diameter: 6,
			core_diameter: 3,
			length: 60
		}
	],
	spindle: [ {rpm: 300} ]
};

var m = new Machine(mill);

m.begin_block("block");
m.exact_path();
m.exact_stop();
m.path_blend();

m.spindle_on(300);
m.motion = "incremental";
m.arc_motion = "incremental";
m.units = "imperial";
m.plane = "ZX";
m.feed_rate_mode = "inverse";
m.feed_rate = 100;

var s = m.spindle;

m.preload_tool(1);
m.tool_change(1);

var t = m.tool;

m.optional_pause();
m.optional_pause("hello");
m.dwell(5);
m.dwell(5, "hello");
m.comment("hello");
m.rapid({x: 100});
m.linear({x: 0});
m.arc();
//m.plunge
m.push_state();
m.push_state();

jprint(m.spindle);
jprint(m.motion);
jprint(m.arc_motion);
jprint(m.units);
jprint(m.plane);
jprint(m.feed_rate_mode);
jprint(m.feed_rate);
jprint(m.tool);

m.discard_state();
m.pop_state();

m.spindle_off();
m.end_block();
var g = m.generate();
jprint(g);

var p = m.generate_model();
jprint(p);

