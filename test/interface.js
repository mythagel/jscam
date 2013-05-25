var mill = {
	type: "mill",
	tools: [],
	spindle: [300]
};

var m = new Machine(mill, "LinuxCNC");

m.exact_path();
m.exact_stop();
m.path_blend();

// TODO

m.motion = "incremental";
m.arc_motion = "incremental";
m.units = "imperial";
m.plane = "UV";
m.feed_rate_mode = "inverse";
m.feed_rate = 100;
//m.spindle_on
//m.spindle_off
//m.load_tool
//m.tool_change
//m.begin_block
//m.end_block]
//m.optional_pause
//m.rapid
//m.linear
//m.arc
//m.plunge
var x = m.generate();
var x = m.generate_model();

