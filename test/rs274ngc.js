var ngc = new rs274ngc();

ngc.units = function(u) {
    print("Units " + u);
}
ngc.linear = function(pos) {
    print("Linear: " + pos.x);
}

ngc.init();

var res = ngc.read("G01 X100 f100");
print(res);
res = ngc.execute();
print(res);
