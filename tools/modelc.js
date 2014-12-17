"use strict";

var decl = JSON.parse(read(args[1]));
print("Compiling stock: " + args[1]);

var model;
if(decl.type == "box") {
    model = make_box(decl.p1, decl.p2);
} else if(decl.type == "cone") {
    // TODO create cone and other stock types
} else {
    throw "Unknown model type."
}

model.write_off(args[1] + ".off");
print("Wrote: " + args[1] + ".off");
