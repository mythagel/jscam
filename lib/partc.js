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
    } else {
        m = decl;
    }
    /* TODO resolve tools in tool table and map to tools
     * in machine object. */

    var machine = new Machine(m, stock);
    return machine;
}

print("Compiling part: " + part.part);

var op_idx = 0;
part.operations.forEach(function(op) {
    ++op_idx;
    var op_id = part.part + " - " + ('000'+op_idx).slice(-3) + op.name;

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
    write(op_id + ".nc", GCODE.generate(gc));
    print("wrote: '" + op_id + ".nc'");

    m.stock.write_off(op_id + ".off");
    print("wrote: '" + op_id + ".off'");

    // Update the stock model based on the result of this operation
    stock.model = m.stock;
});

