load("lathe.js")
init(lathe, "LinuxCNC");

units("imperial");
feed_rate(100);
exact_stop();
units("metric");
feed_rate(100);
exact_path();

motion("incremental");
