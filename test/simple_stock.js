"use strict";

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZA",
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

var stock = {};
stock.model = make_box({x:0, y:0, z:0}, {x:50, y:50, z:10});

var m = new Machine(mill, stock);

m.feed_rate = 100;
m.tool_change(1);
m.spindle_on(250);

m.rapid({z:11});
m.rapid({x:0, y:0});

m.linear({z:9});
m.linear({x:50, y:50});

m.stock.write_off("simple_stock.off");
