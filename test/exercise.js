var config = {
	type: "mill",
	tools:
	[
		{
			name: "3mm End Mill",
			id: 4,
			type: "mill",
		},
		{
			name: "1mm Ball End Mill",
			id: 5,
			type: "mill",
		}
	],
	spindle: [ 100, 250]
}
init(config, "LinuxCNC");

begin_block("Setup");
// Roughing
path_blend(0.01, 0.01);

begin_block("Program");
optional_pause("Pause before program");
tool_change(5);
load_tool(4);
//m.Rapid(Y(5), X(6), A(1.0/3.0));
tool_change(4);
spindle_on(300);

begin_block("Positioning");
spindle_off();
tool_change(5);
motion("incremental");
//m.Rapid(X(1), Y(1));
//m.Rapid(X(1));
//m.Rapid(X(1));
//m.Rapid(X(1), Z(2));
//m.Rapid(X(1));
end_block("spindle"); // Don't restore tool


begin_block("Cutting");
spindle_on(100);
feed_rate(100);
motion("absolute");
//m.Rapid(X(0), Y(0), Z(0));
//m.Linear(X(5), Y(5), Z(5));

feed_rate_mode("inverse_time");
//m.Linear(X(5), Y(5), Z(5));

end_block("restore");

begin_block("Epilogue");
spindle_off();

/*
{
	Machine m(Machine::Type::Lathe, "LinuxCNC");

	m.SetUnits(Machine::Units::Imperial);
	m.SetFeedRate(100);
	m.AccuracyExactStop();
	m.SetUnits(Machine::Units::Metric);
	m.SetFeedRate(100);
	m.AccuracyExactPath();

	m.SetMotion(Machine::Motion::Incremental);
}
*/
