"use strict";

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZ",
	tools:
	{
		1: {
			name: "3mm End Mill",
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
	},
	spindle: [ {rpm: [100, 1000] }, {rpm: [2000, 6000]}]
};

var m = new Machine(mill);

m.begin_block("Setup");
	m.feed_rate = 100;
	m.spindle_on(250);
	m.preload_tool(1);
	m.tool_change(1);
	m.optional_pause("Post Setup");
m.end_block("tool", "feedrate");


