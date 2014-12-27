"use strict";

var RS274NGC_OK = 0;

var steps = [];
var ngc = new rs274ngc();
ngc.on_offset_origin = function(pos) {
//    print("offset_origin: " + JSON.stringify(pos));
}

ngc.on_units = function(u) {
}
ngc.on_plane = function(pl) {
}
ngc.on_rapid_rate = function(rate) {
}
ngc.on_feed_rate = function(rate) {
}
ngc.on_feed_reference = function(reference) {
}
ngc.on_motion_mode = function(mode) {
}
ngc.on_cutter_radius_comp = function(radius) {
}
ngc.on_cutter_radius_comp_start = function(direction) {
}
ngc.on_cutter_radius_comp_stop = function() {
}
ngc.on_speed_feed_sync_start = function() {
}
ngc.on_speed_feed_sync_stop = function() {
}

ngc.on_rapid = function(pos) {
    var path = expand_linear(ngc.current_position(), pos, "XYZABC", 10);
    path.motion = "rapid";
    steps.push(path);
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

    var path = expand_arc(ngc.current_position(), end, center, loops<0, plane, Math.abs(loops), "XYZABC", 10);
    path.motion = "arc";
    steps.push(path);
}
ngc.on_linear = function(pos) {
    var path = expand_linear(ngc.current_position(), pos, "XYZABC", 10);
    path.motion = "linear";
    steps.push(path);
}
ngc.on_probe = function(pos) {
}
ngc.on_dwell = function(seconds) {
}

ngc.on_spindle_start_clockwise = function() {
}
ngc.on_spindle_start_counterclockwise = function() {
}
ngc.on_spindle_stop = function() {
}
ngc.on_spindle_speed = function(r) {
}
ngc.on_spindle_orient = function(orientation, direction) {
}
ngc.on_tool_length_offset = function(length) {
}
ngc.on_tool_change = function(slot) {
}
ngc.on_tool_select = function(i) {
}
ngc.on_axis_clamp = function(axis) {
}
ngc.on_axis_unclamp = function(axis) {
}
ngc.on_comment = function(s) {
}
ngc.on_feed_override_disable = function() {
}
ngc.on_feed_override_enable = function() {
}
ngc.on_speed_override_disable = function() {
}
ngc.on_speed_override_enable = function() {
}
ngc.on_coolant_flood_off = function() {
}
ngc.on_coolant_flood_on = function() {
}
ngc.on_coolant_mist_off = function() {
}
ngc.on_coolant_mist_on = function() {
}
ngc.on_message = function(s) {
}
ngc.on_pallet_shuttle = function() {
}
ngc.on_probe_off = function() {
}
ngc.on_probe_on = function() {
}
ngc.on_program_optional_stop = function() {
}
ngc.on_program_end = function() {
}
ngc.on_program_stop = function() {
}

var lines = read(args[1]).split("\n");

ngc.init();

lines.forEach(function(line) {
    var status = ngc.read(line);
    if(status != RS274NGC_OK) {
        print("Error reading line: " + line + "\n");
        return status;
    }
    status = ngc.execute();
    if(status != RS274NGC_OK)
        return status;
});

ngc.exit();

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
