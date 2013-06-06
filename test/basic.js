"use strict";

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZ",
	tools: [],
	spindle: ["100-1000", "2000-6000"]
};

var m = new Machine(mill);

print(__filename);
