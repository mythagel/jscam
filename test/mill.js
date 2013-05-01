var config = {
	type: "mill",
	tools:
	[
		{
			name: "3mm End Mill",
			id: 0,
			type: "mill",
		}
	],
	spindle: ["100-1000", "2000-6000"]
}
init(config, "LinuxCNC");
