var config = {
	type: "mill",
	tools:
	[
		{
			name: "0.8mm End Mill",
			id: 1,
			type: "mill",

			center_cutting: true,
			flutes: 4,
			flute_length: 10,
			cutting_length: 10,
			mill_diameter: 0.8,
			shank_diameter: 1,
			core_diameter: 0.6,
			length: 40
		}
	],
	spindle: [ "0-100", 300, "500-1000", 3000]
}
init(config, "LinuxCNC");

begin_block("Setup");
	feed_rate(100);
	spindle_on(250);
	load_tool(1);
	tool_change(1);
	optional_pause("Post Setup");
end_block();


