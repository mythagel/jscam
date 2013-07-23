"use strict";

load("machine.js");

var mill = {
	type: Machine.Type.Mill,
	tools:
	{
		1: {
			name: "3mm End Mill",
			type: Machine.Type.Mill,
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
	spindle: ["100-1000", "2000-6000"]
};

