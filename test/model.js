var mill = {
	type: "mill",
	tools: [],
	spindle: [300]
};

var m = new Machine(mill, "LinuxCNC");
var model = m.generate_model();
//print(JSON.stringify(model));