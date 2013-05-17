load("mill.js");
init(mill, "LinuxCNC");

/*
pocket.js
=========

Lessons learnt in planning already:
Need to be able to access properties from cxxcam, i.e. active tool diameter.
*/
function rubbish()
{
	begin_block("Rectangular Pocket dimensions...");
	for(var x = 0; x != 10 ; ++x)
	{
		rapid({z: 5});
		rapid({x: x, y: 0});
		if(x % 2 == 0)
			linear({z: -1});
		else
			linear({z: -2});
		linear({x: x, y: 10});
	}
	
	end_block(machine.BlockRestore.RestoreAll);
}

function polygon(sides, size, center, depth)
{
	begin_block("polygon");
	rapid ({x: center.x + size * Math.cos(0), y: center.y + size * Math.sin(0)});         
	linear({z: depth});

	for (var side = 1; side <= sides; ++side)
	{
		var x = center.x + size * Math.cos(side * 2 * Math.PI / sides);
		var y = center.y + size * Math.sin(side * 2 * Math.PI / sides);
		linear({x: x, y: y});
	}
	end_block(machine.BlockRestore.RestoreAll);
}

begin_block("main");
feed_rate(100);
spindle_on(100);
rubbish();
end_block(machine.BlockRestore.RestoreAll);

begin_block("polygon test");

feed_rate(100);
spindle_on(100);

polygon(6, 20, {x: 10, y: 10}, -1);
polygon(5000, 100, {x: 0, y: 0}, -1);

end_block(machine.BlockRestore.RestoreAll);
