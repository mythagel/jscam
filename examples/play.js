"use strict";
load("../lib/gcode.js");

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

var ngc = new rs274ngc();
ngc.on_units = function(u) {
    print("Units " + u);
}
ngc.on_linear = function(pos) {
    print("Linear: " + pos.x);
}
ngc.init();


mill.on_gcode = function(line) {
    print(JSON.stringify(line));
    var res = ngc.read(GCODE.generate_line(line));
    res = ngc.execute();
}

var m = new Machine(mill, stock);

m.feed_rate = 100;
m.tool_change(1);
m.spindle_on(250);

m.rapid({z:11});
m.rapid({x:0, y:0});

//for(var i = 0; i < 11; ++i)
//{
//	print("{x:" + 5*i + ", y:" + 5*i + ", z:" + (10-i) + "}")
//	m.linear({z:10-i});
//	m.linear({x:5*i, y:5*i});

//	//m.linear({x:5*i, y:5*i, z:10-i});
//}

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

polygon(6, 10, {x:25, y:25}, 3);

m.stock.write_off("play.off");
print(GCODE.generate(m.generate()));
