"use strict";
load("../lib/gcode.js");

var mill = {
	type: "mill",
	units: "metric",
	axes: "XYZA",
	tools:
	{
		1: {
			name: "3mm End Mill",
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
	},
	spindle: [ {rpm: [100, 1000] }, {rpm: [2000, 6000]}]
};

var stock = {};
stock.model = make_box({x:0, y:0, z:0}, {x:50, y:50, z:10});

var steps = [];

var ngc = new rs274ngc();
ngc.on_rapid = function(pos) {
    var path = expand_linear(ngc.current_position(), pos, mill.axes, 10);
    path.motion = "rapid";
    steps.push(path);
//    print(JSON.stringify(path));
}
ngc.on_linear = function(pos) {
    var path = expand_linear(ngc.current_position(), pos, mill.axes, 10);
    path.motion = "linear";
    steps.push(path);
//    print(JSON.stringify(path));
}
ngc.on_arc = function(end0, end1, axis0, axis1, loops, end_point, a, b, c) {
    var end = {};
    var center = {x:0,y:0,z:0};
    var plane = {x:0,y:0,z:0};

    switch(ngc.plane()) {
        case ngc.Plane.XY:
            end.x = end0;
            end.y = end1;
            end.z = end_point;
            center.x = axis0;
            center.y = axis1;
            plane.z = 1;
            break;
        case ngc.Plane.YZ:
            end.x = end_point;
            end.y = end0;
            end.z = end1;
            center.y = axis0;
            center.z = axis1;
            plane.x = 1;
            break;
        case ngc.Plane.XZ:
            end.x = end1;
            end.y = end_point;
            end.z = end0;
            center.x = axis1;
            center.z = axis0;
            plane.y = 1;
            break;
    }
    end.a = a;
    end.b = b;
    end.c = c;

    var path = expand_arc(ngc.current_position(), end, center, loops<0, plane, Math.abs(loops), mill.axes, 10);
    path.motion = "arc";
    steps.push(path);
//    print(JSON.stringify(path));
}
ngc.init();


mill.on_gcode = function(line) {
    var code = GCODE.generate_line(line);
    print(code)
    var res = ngc.read(code);
    res = ngc.execute();
}

var m = new Machine(mill, stock);

m.feed_rate = 100;
m.tool_change(1);
m.spindle_on(250);

m.rapid({z:11});
m.rapid({x:0, y:0});

m.rapid({x:25, y:25});
m.arc("clockwise", {x: 25, y: 25, z: 3, i:-2, j:-2, turns: 5});

//for(var i = 0; i < 11; ++i)
//{
//	print("{x:" + 5*i + ", y:" + 5*i + ", z:" + (10-i) + "}")
//	m.linear({z:10-i});
//	m.linear({x:5*i, y:5*i});

//	//m.linear({x:5*i, y:5*i, z:10-i});
//}

function polygon(sides, size, center, depth)
{
	var tool = m.tool;
	//tool.mill_diameter
	// TODO spiral in pocket by mill width steps
	
	m.rapid ({x: center.x + size * Math.cos(0), y: center.y + size * Math.sin(0)});
	m.linear({z: depth});

	for (var side = 1; side <= sides; ++side)
	{
		var x = center.x + size * Math.cos(side * 2 * Math.PI / sides);
		var y = center.y + size * Math.sin(side * 2 * Math.PI / sides);
		m.linear({x: x, y: y});
	}
}

polygon(6, 10, {x:25, y:25}, 3);

m.stock.write_off("play.off");
//print(GCODE.generate(m.generate()));

print("function model(scene) {");
steps.forEach(function(step) {
    print("  {");
    print("    var geometry = new THREE.Geometry();");
    step.path.forEach(function(pos) {
        print("    geometry.vertices.push(new THREE.Vector3(" + pos.pos.x + "," + pos.pos.y + "," + pos.pos.z + "));");
    });
    if(step.motion == "rapid")
        print("    var line = new THREE.Line(geometry, red);");
    else
        print("    var line = new THREE.Line(geometry, green);");
    print("    scene.add(line);");
    print("  }");
});
print("}");
