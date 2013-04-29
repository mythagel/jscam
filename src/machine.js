machine.Type
{
	Mill  : "mill",
	Lathe : "lathe"
}

machine.BlockRestore
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

machine.Units
{
	Metric   : "metric",
	Imperial : "imperial"
}

machine.Plane
{
    XY : "XY",
    ZX : "ZX",
    YZ : "YZ",
    UV : "UV",
    WU : "WU",
    VW : "VW"
}

machine.Motion
{
	Absolute    : "absoloute",
	Incremental : "incremental"
}

machine.FeedRateMode
{
	InverseTime        : "inverse",
	UnitsPerMinute     : "upm",
	UnitsPerRevolution : "upr"
}

machine.Rotation
{
	Stop             : "stop",
	Clockwise        : "clockwise",
	CounterClockwise : "counterclockwise"
}

machine.Direction
{
	Clockwise        : "clockwise",
	CounterClockwise : "counterclockwise"
}
