var ngc = new rs274ngc();

ngc.on_units = function(u) {
    print("Units " + u);
}
ngc.on_linear = function(pos) {
    print("Linear: " + pos.x);
}

ngc.init();

print("XXXX: " + ngc.Units.METRIC);

var res = ngc.read("G01 X100 f100");
print(res);
res = ngc.execute();
print(res);
