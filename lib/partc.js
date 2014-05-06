"use strict";
load("gcode.js");

/* Declcam Part compiler
 * called as jscam partc.js the_part.part
 * */

var part = JSON.parse(read(args[1]));
var machine_library;
var tool_library;
var ops_library;

var len = args.length;
for (var i = 0; i < len; ++i) {
    var arg = args[i];
    if(arg == "--machines") {
        machine_library = args[++i];
    } else if(arg == "--tools") {
        tool_library = args[++i];
    } else if(arg == "--ops") {
        ops_library = args[++i];
    }
}

var stock = {};
if(part.stock.stock == "box") {
    stock.model = make_box(part.stock.p1, part.stock.p2);
} else if(part.stock.stock == "cone") {
    // TODO create cone and other stock types
} else {
    throw "Unknown stock type."
}

function build_machine(decl, tool_table, stock) {
    var m;
    if (typeof decl == 'string') {
        m = JSON.parse(read(machine_library + "/" + decl + ".machine"));
        print(JSON.stringify(m));
    } else {
        m = decl;
    }
    /* TODO resolve tools in tool table and map to tools
     * in machine object. */

    var machine = new Machine(m, stock);
    return machine;
}

part.operations.forEach(function(op) {
    print(op.name);
    if(op.machine === undefined)
        op.machine = part.machine;
    if(op["tool-table"] === undefined)
        op["tool-table"] = part["tool-table"]

    var m = build_machine(op.machine, op["tool-table"], stock);

    m.begin_block("prelude");
    m.comment("Part       : " + part.part);
    m.comment("Operation  : " + op.name);
    m.comment("Description: " + part.description);
    m.comment("Machine    : " + part.machine);
    m.comment("Material   : " + part.material);
    m.end_block();

    load(ops_library + "/" + op.operation + ".js");
    OP[op.operation](part, op, m);

    var gc = m.generate();
    write(part.part + " - " + op.name + ".nc", GCODE.generate(gc));
    print("wrote: '" + part.part + " - " + op.name + ".nc'");

    m.stock.write_off(part.part + " - " + op.name + ".off");
    print("wrote: '" + part.part + " - " + op.name + ".off'");

    // Update the stock model based on the result of this operation
    stock.model = m.stock;
});

/* TODO the below code is mostly just testing.
 * Elements of this will flow into the final design, but
 * parts are defined by a set of operations, not globally,
 * and each operation may be on a different machine.
 * */

var test =
{
    machine: "Sherline 4-Axis Mill",
	type: "mill",
    units: "metric",
    axes: "XYZA",
    tools: {},
	spindle: [ {rpm: [70, 2800], nm: [3.1776, 0.0706]} ],
	feed_rate: {z: 50, max: 300},
	rapid_rate: {z: 50, max: 300}
};

var m = build_machine(test, {}, stock);
print("Compiling part: " + part.part);

m.begin_block("prelude");
m.comment("Part       : " + part.part);
m.comment("Description: " + part.description);
m.comment("Machine    : " + part.machine);
m.comment("Material   : " + part.material);
m.end_block();

var gc = m.generate();
write(part.part + ".nc", GCODE.generate(gc));
print("wrote: '" + part.part + ".nc'");

m.stock.write_off(part.part + ".off");
print("wrote: '" + part.part + ".off'");
