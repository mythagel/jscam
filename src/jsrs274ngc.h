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

var Position = {
    x:0,y:0,z:0,
    a:0,b:0,c:0
};

Units = {
    Imperial: 0,
    Metric: 1
};

Plane = {
	XY: 0,
	YZ: 1,
	XZ: 2
};

FeedReference = {
	Workpiece: 0,
	XYZ: 1
};

Motion = {
	Exact_Stop: 0,
	Exact_Path: 1,
	Continuous: 2
};

Direction = {
	Stop: 0,
	Clockwise: 1,
	CounterClockwise: 2
};

var Tool = {
    id: 0;
    length: 0.0;
    diameter: 0.0;
};

Axis = {
	X:0, Y:1, Z:2,
	A:3, B:4, C:5
};

rs274ngc
{
	void init()

	void offset_origin(const Position& pos)
	
	void units(Units u)
	Units units() const
	
	void plane(Plane pl)
	Plane plane() const
	
	void rapid_rate(double rate)
	double rapid_rate() const
	
	void feed_rate(double rate)
	double feed_rate() const
	void feed_reference(FeedReference reference)
	
	void motion_mode(Motion mode)
	Motion motion_mode() const
	
	void cutter_radius_comp(double radius)
	void cutter_radius_comp_start(Side direction)
	void cutter_radius_comp_stop()
	
	void speed_feed_sync_start()
	void speed_feed_sync_stop()
	
	void rapid(const Position& pos)
	void arc(double end0, double end1, double axis0, double axis1, int rotation, double end_point, double a, double b, double c)
	void linear(const Position& pos)
	void probe(const Position& pos)
	void dwell(double seconds)

	void spindle_start_clockwise()
	void spindle_start_counterclockwise()
	void spindle_stop()
	Direction spindle_state() const
	void spindle_speed(double r)
	double spindle_speed() const
	void spindle_orient(double orientation, Direction direction)

	void tool_length_offset(double length)
	void tool_change(int slot)
	void tool_select(int i)
	int tool_slot() const
	Tool tool(int pocket) const
	unsigned int tool_max() const

	void axis_clamp(Axis axis)
	void axis_unclamp(Axis axis)

	void comment(const char *s)

	void feed_override_disable()
	void feed_override_enable()

	void speed_override_disable()
	void speed_override_enable()

	void coolant_flood_off()
	void coolant_flood_on()
	bool coolant_flood() const
	
	void coolant_mist_off()
	void coolant_mist_on()
	bool coolant_mist() const

	void message(const char *s)

	void pallet_shuttle()

	void probe_off()
	void probe_on()
	Position probe_position() const
	double probe_value() const

	void program_optional_stop()
	void program_end()
	void program_stop()

	void get_parameter_filename(char* filename, size_t max_size) const
	Position current_position() const
	bool queue_empty() const
	
public:
	void synch();
	int read(const char * command);
	int execute();
	void reset();
	void exit();
	void load_tool_table();

	[0-11] active_g_codes(int * codes);
	[0-6] active_m_codes(int * codes);
	[0-2] active_settings(double * settings);

	string line_text();
}

*/
void bind(v8::Handle<v8::Object> global);

}

#endif /* JSRS274NGC_H_ */
