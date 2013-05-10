var config = {
	type: "mill",
	tools:
	[
		{
			name: "3mm End Mill",
			id: 4,
			type: "mill",
			center_cutting: true,
			flutes: 4,
			flute_length: 25,
			cutting_length: 20,
			mill_diameter: 3,
			shank_diameter: 6,
			core_diameter: 3,
			length: 60
		},
		{
			name: "1mm Ball End Mill",
			id: 5,
			type: "mill",
			center_cutting: true,
			flutes: 4,
			flute_length: 12,
			cutting_length: 10,
			mill_diameter: 1,
			shank_diameter: 3,
			core_diameter: 0.5,
			length: 40
		}
	],
	spindle: [ 100, 250]
}
init(config, "LinuxCNC");

begin_block("Setup");
// Roughing
path_blend(0.01, 0.01);

begin_block("Program");
optional_pause("Pause before program");
tool_change(5);
load_tool(4);
rapid({y: 5, x: 6, a: (1.0/3.0) });
tool_change(4);
spindle_on(300);

begin_block("Positioning");
spindle_off();
tool_change(5);
motion("incremental");
rapid({x: 1}, {y: 1});
rapid({x: 1});
rapid({x: 1});
rapid({x: 1, z: 2});
rapid({x: 1});
end_block("spindle"); // Don't restore tool


begin_block("Cutting");
spindle_on(100);
feed_rate(100);
motion("absolute");
rapid({x: 0, y: 0, z: 0});
linear({x: 5, y: 5, z: 5});

feed_rate_mode("inverse");
linear({x: 5, y: 5, z: 5});

end_block("restore");

begin_block("Epilogue");
spindle_off();


{
	var config = {
		type: "lathe",
		tools: [],
		spindle: [100, 250]
	}
	init(config, "LinuxCNC");

	units("imperial");
	feed_rate(100);
	exact_stop();
	units("metric");
	feed_rate(100);
	exact_path();

	motion("incremental");
}

