/* jscam - C++ CAD/CAM driver library.
 * Copyright (C) 2014  Nicholas Gill
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * jsrs274ngc.h
 *
 *  Created on: 2014-12-09
 *      Author: nicholas
 */

#ifndef JSRS274NGC_H_
#define JSRS274NGC_H_
#include <v8.h>

namespace jsrs274ngc
{

/*
API Sketch

// Types
var Position = {
    x:0,y:0,z:0,
    a:0,b:0,c:0
};
var Tool = {
    id: 0;
    length: 0.0;
    diameter: 0.0;
};

rs274ngc
{
    Units = {
        IMPERIAL: 0,
        METRIC: 1
    };

    Plane = {
	    XY: 0,
	    YZ: 1,
	    XZ: 2
    };

    FeedReference = {
	    WORKPIECE: 0,
	    XYZ: 1
    };

    Motion = {
	    EXACT_STOP: 0,
	    EXACT_PATH: 1,
	    CONTINUOUS: 2
    };

    Direction = {
	    STOP: 0,
	    CLOCKWISE: 1,
	    COUNTERCLOCKWISE: 2
    };

    Axis = {
	    X:0, Y:1, Z:2,
	    A:3, B:4, C:5
    };

	void init();
	void synch();
	int read(const char * command);
	int execute();
	void reset();
	void exit();
	void load_tool_table();
	[0-11] active_g_codes();
	[0-6] active_m_codes();
	[0-2] active_settings();
	string line_text();

	Units units()
	Plane plane()
	double rapid_rate()
	double feed_rate()
	Motion motion_mode()
	Direction spindle_state()
	double spindle_speed()
	int tool_slot()
	bool coolant_flood()
	bool coolant_mist()
	Position probe_position()
	Position current_position()
	bool queue_empty()

    // functions set by js interface
	unsigned int get_tool_max()
	Tool get_tool(int pocket)
	double get_probe_value()
	void get_parameter_filename(char* filename, size_t max_size)
	
	// callbacks if set
	void on_offset_origin(const Position& pos)
	
	void on_units(Units u)
	
	void on_plane(Plane pl)
	
	void on_rapid_rate(double rate)
	
	void on_feed_rate(double rate)
	void on_feed_reference(FeedReference reference)
	
	void on_motion_mode(Motion mode)
	
	void on_cutter_radius_comp(double radius)
	void on_cutter_radius_comp_start(Side direction)
	void on_cutter_radius_comp_stop()
	
	void on_speed_feed_sync_start()
	void on_speed_feed_sync_stop()
	
	void on_rapid(const Position& pos)
	void on_arc(double end0, double end1, double axis0, double axis1, int rotation, double end_point, double a, double b, double c)
	void on_linear(const Position& pos)
	void on_probe(const Position& pos)
	void on_dwell(double seconds)

	void on_spindle_start_clockwise()
	void on_spindle_start_counterclockwise()
	void on_spindle_stop()
	void on_spindle_speed(double r)
	void on_spindle_orient(double orientation, Direction direction)

	void on_tool_length_offset(double length)
	void on_tool_change(int slot)
	void on_tool_select(int i)

	void on_axis_clamp(Axis axis)
	void on_axis_unclamp(Axis axis)

	void on_comment(const char *s)

	void on_feed_override_disable()
	void on_feed_override_enable()

	void on_speed_override_disable()
	void on_speed_override_enable()

	void on_coolant_flood_off()
	void on_coolant_flood_on()
	
	void on_coolant_mist_off()
	void on_coolant_mist_on()

	void on_message(const char *s)

	void on_pallet_shuttle()

	void on_probe_off()
	void on_probe_on()

	void on_program_optional_stop()
	void on_program_end()
	void on_program_stop()
}

*/
void bind(v8::Handle<v8::Object> global);

}

#endif /* JSRS274NGC_H_ */
