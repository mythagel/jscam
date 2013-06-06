"use strict";

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZ",
	tools: [],
	spindle: [300]
};

var m = new Machine(mill);
var model = m.generate_model();
//print(JSON.stringify(model));
