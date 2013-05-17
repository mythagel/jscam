load("mill.js");
init(mill, "LinuxCNC");

/*
pocket.js
=========
*/
function pocket()
{
	begin_block("Rectangular Pocket dimensions...");
	// TODO
	end_block(machine.BlockRestore.restore);
}

pocket();

