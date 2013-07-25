"use strict";

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZA",
	tools:
	{
		1: {
			name: "1mm End Mill",
			type: "mill",
			center_cutting: true,
			flutes: 4,
			flute_length: 25,
			cutting_length: 20,
			mill_diameter: 1,
			shank_diameter: 2,
			core_diameter: 0.5,
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

function polygon(sides, size, center, depth)
{
	var tool = m.tool;
	//tool.mill_diameter
	// TODO spiral in pocket by mill width steps
	
	m.rapid ({x: center.x + size * Math.cos(0), y: center.y + size * Math.sin(0)});
	m.linear({z: depth});

	for (var side = 1; side <= sides; ++side)
	{
		var x = center.x + size * Math.cos(side * 2 * Math.PI / sides);
		var y = center.y + size * Math.sin(side * 2 * Math.PI / sides);
		m.linear({x: x, y: y});
	}
}

for(var i = 0; i < 10; ++i)
polygon(6, 1, {x:10, y:10 + (4 * i)}, 8);

m.stock.write_off("hexgrid.off");
