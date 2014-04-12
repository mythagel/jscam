"use strict";

var poly = make_box({x:0, y:0, z:0}, {x:50, y:50, z:10});
var bbox = bounding_box(poly);

print(JSON.stringify(bbox))
