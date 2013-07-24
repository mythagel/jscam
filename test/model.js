"use strict";

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZ",
	tools: {},
	spindle: [300]
};

var m = new Machine(mill);
var model = m.stock.to_object();
print(JSON.stringify(model));
