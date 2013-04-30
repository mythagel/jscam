jscam
=====

js shell for cxxcam driver

Under active development.

Example
=======

    var config = {
        type: "mill",
        tools:
        [
            {
                name: "0.8mm End Mill",
                id: 1,
                type: "mill",
    
                center_cutting: true,
                flutes: 4,
                flute_length: 10,
                cutting_length: 10,
                mill_diameter: 0.8,
                shank_diameter: 1,
                core_diameter: 0.6,
                length: 40
            }
        ],
        spindle: [ "0-100", 300, "500-1000", 3000]
    }
    init(config, "LinuxCNC");
    
    begin_block("Setup");
        feed_rate(100);
        spindle_on(250);
        load_tool(1);
        tool_change(1);
        optional_pause("Post Setup");
    end_block("tool", "feedrate");

Generated output
================
    ; cxxcam  Mill  XY  Metric  Absolute  Incremental Arc  Units Per Minute  
    G0 G17 G21 G40 G49 G54 G80 G90 G91.1 G94 G97 M9 M5
    
    ; Setup
    F100 ; 100mm per minute
    M3 S300 ; Start Spindle Clockwise 300 RPM (250 RPM Requested)
    T1 ; Preload tool 0.8mm End Mill
    T1 M6 ; Switch to tool 0.8mm End Mill
    M1 ; Post Setup
    ; Restore State
    F0
    T0 M6 ; Empty Spindle
    
    M2 ; End of program.
