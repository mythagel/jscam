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
stock.model = make_cone({x:-30, y:0, z:0}, {x:0, y:0, z:0}, 20, 20, 32);
//stock.model = stock.model.rotate({w:0.70710, x:0, y:0, z:0.70710});

var m = new Machine(mill, stock);

m.feed_rate = 100;
m.tool_change(1);
m.spindle_on(250);

m.rapid({x:-40, y:0, z:20});

m.linear({z: 8});
m.motion = "incremental";
for(var i = 0; i < 10; ++i)
{
    m.linear({x: 50});
    m.rapid({a: 360/10});
    m.linear({x: -50});
}

//m.linear({z: -2});
//m.linear({x: 10});
//m.linear({x: -10});
//m.rapid({b: 20});

m.stock.write_off("fan.off");
