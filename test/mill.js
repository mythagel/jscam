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
		}
	],
	spindle: ["100-1000", "2000-6000"]
}
init(config, "LinuxCNC");
