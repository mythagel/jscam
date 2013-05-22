load("mill.js");

var m = new Machine(mill, "LinuxCNC");

/*
pocket.js
=========

Lessons learnt in planning already:
Need to be able to access properties from cxxcam, i.e. active tool diameter.
*/

function polygon(sides, size, center, depth)
{
	m.begin_block("polygon od " + size + " @ (" + center.x + "," + center.y + ")");
	m.rapid ({x: center.x + size * Math.cos(0), y: center.y + size * Math.sin(0)});         
	m.linear({z: depth});

	for (var side = 1; side <= sides; ++side)
	{
		var x = center.x + size * Math.cos(side * 2 * Math.PI / sides);
		var y = center.y + size * Math.sin(side * 2 * Math.PI / sides);
		m.linear({x: x, y: y});
	}
	m.end_block(machine.BlockRestore.RestoreAll);
}

m.begin_block("polygon test");

m.feed_rate(100);
m.spindle_on(100);

m.begin_block("polygon grid");
for(var i = 0; i < 10; ++i)
{
	polygon(6, 10, {x: (20*i) + 5, y: 0}, -1);
}
m.end_block();

m.end_block(machine.BlockRestore.RestoreAll);

print(JSON.stringify(m.generate()));

m = null;
