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

function make_machine(decl, tool_table, stock) {
    var m;
    if (typeof decl == 'string') {
        m = JSON.parse(read(machine_library + "/" + decl + ".machine"));
    } else {
        m = decl;
    }

    for (var id in tool_table) {
        if (typeof tool_table[id] == 'string') {
            m.tools[id] = JSON.parse(read(tool_library + "/" + tool_table[id] + ".tool"));
        } else {
            m.tools[id] = tool_table[id];
        }
    }

    var machine = new Machine(m, stock);
    return machine;
}

print("Compiling part: " + part.part);

var setup_index = 0;
part.setups.forEach(function(setup) {
    ++setup_index;

    var id = part.part;
    if(part.setups.length > 1)
        id = id + "-" + ('000' + setup_index).slice(-3);
    if(setup.name)
        id = id + "-" + setup.name;
    var machine = make_machine(setup.machine, setup.tool_table, stock);

    machine.begin_block("Information");
    machine.comment("Part       : " + part.part);
    machine.comment("Description: " + part.description);
    machine.comment("Material   : " + part.material);
    machine.comment("Machine    : " + setup.machine);
    if(setup.name)
        machine.comment("Setup      : " + setup.name);
    machine.end_block();

    setup.operations.forEach(function(op) {
        machine.begin_block(op.name);
        load(ops_library + "/" + op.operation + ".js");
        OP[op.operation](part, op, machine);
        machine.end_block("restore");

        // Update the stock model based on the result of this operation
        stock.model = machine.stock;
    });

    var gc = machine.generate();
    write(id + ".nc", GCODE.generate(gc));
    print("wrote: '" + id + ".nc'");

    machine.stock.write_off(id + ".off");
    print("wrote: '" + id + ".off'");
});

