"use strict";

load("sherline.js");

load("../lib/spiral.js");
load("../lib/gcode.js");

load("load_json.js");

mill.tools[1] = load_json("tools/HSS 2mm 2 flute End Mill.json");

var al_smpm = 75;

function spindle_speed(smpm, d)
{
    return (smpm * 318.057)  / d;
}
function feed_rate(rpm, clpt, flutes)
{
    return rpm * clpt * flutes;
}
function chip_load_per_tooth(f, d)
{
    return f * d;
}

var stock = {};
stock.model = make_box({x:-50, y:-50, z:-10}, {x:50, y:50, z:0});

var m = new Machine(mill, stock);

m.begin_block("spiral test");
m.tool_change(1);

var rpm = spindle_speed(al_smpm, m.tool.mill_diameter);
// Limit rpm to max of sherline spindle to bypass out of range errors from cxxcam
if(rpm > 2800)
    rpm = 2800;

m.spindle_on(rpm);
var clpt = chip_load_per_tooth(.0256, m.tool.mill_diameter);
var mmpm = feed_rate(m.spindle.speed, clpt, m.tool.flutes);
m.feed_rate = mmpm;

m.linear({z: -2});
spiral(m, {x: 0, y: 0}, 32, 15, 0, 0.8, 0);
m.end_block(Machine.BlockRestore.RestoreAll);

write("spiral_test.ngc", GCODE.generate(m.generate()));
m.stock.write_off("spiral_test.off");
