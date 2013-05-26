//Machine = {};

Machine.Type =
{
	Mill  : "mill",
	Lathe : "lathe"
}

Machine.BlockRestore =
{
	Preserve : "preserve",
	RestoreAll : "restore",
	RestoreUnits  : "units",
	RestorePlane : "plane",
	RestoreMotion : "motion",
	RestoreArcMotion : "arc_motion",
	RestoreFeedRateMode : "feedrate_mode",
	RestoreFeedRate : "feedrate",
	RestoreSpindle : "spindle",
	RestoreTool : "tool",
	RestorePosition : "position"
}

Machine.Units =
{
	Metric   : "metric",
	Imperial : "imperial"
}

Machine.Plane =
{
    XY : "XY",
    ZX : "ZX",
    YZ : "YZ",
    UV : "UV",
    WU : "WU",
    VW : "VW"
}

Machine.Motion =
{
	Absolute    : "absolute",
	Incremental : "incremental"
}

Machine.FeedRateMode =
{
	InverseTime        : "inverse",
	UnitsPerMinute     : "upm",
	UnitsPerRevolution : "upr"
}

Machine.Rotation =
{
	Stop             : "stop",
	Clockwise        : "clockwise",
	CounterClockwise : "counterclockwise"
}

Machine.Direction =
{
	Clockwise        : "clockwise",
	CounterClockwise : "counterclockwise"
}

