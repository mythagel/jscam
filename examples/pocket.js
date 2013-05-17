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

begin_block("main");
feed_rate(100);
spindle_on(100);
rubbish();
end_block(machine.BlockRestore.RestoreAll);
