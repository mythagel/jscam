load("lathe.js")

var m = new Machine(lathe, "LinuxCNC");

m.units = "imperial";
m.feed_rate = 100;
m.exact_stop();
m.units = "metric";
m.feed_rate = 100;
m.exact_path();

m.motion = "incremental";

print(JSON.stringify(m.generate()));
