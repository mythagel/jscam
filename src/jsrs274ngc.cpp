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
 * jsrs274ngc.cpp
 *
 *  Created on: 2014-12-09
 *      Author: nicholas
 */

#include "jsrs274ngc.h"
#include "js.h"
#include <memory>
#include <vector>
#include <cmath>
#include <cstring>
#include "rs274ngc/rs274ngc.hh"

using namespace v8;

using js::operator"" _sym;

namespace jsrs274ngc
{

class js_rs274ngc : public rs274ngc
{
private:
    Persistent<Object> object;

	Plane       _active_plane = Plane::XY;
	int               _active_slot = 1;
	double            _feed_rate = 0.0;
	int               _flood = 0;
	int               _mist = 0;
	Units       _length_unit_type = Units::Metric;
	Motion _motion_mode = Motion::Continuous;

	Position probe_pos;
	Position origin_pos;
	Position program_pos;

	double            _spindle_speed = 0.0;
	Direction   _spindle_turning = Direction::Stop;
	double            _traverse_rate = 60;

private:

	virtual void interp_init() override
	{
	}

    static Handle<Object> wrap_Position(const Position& pos)
    {
		auto jspos = Object::New();
        jspos->Set("x"_sym, Number::New(pos.x));
        jspos->Set("y"_sym, Number::New(pos.y));
        jspos->Set("z"_sym, Number::New(pos.z));
        jspos->Set("a"_sym, Number::New(pos.a));
        jspos->Set("b"_sym, Number::New(pos.b));
        jspos->Set("c"_sym, Number::New(pos.c));
        return jspos;
    }

	virtual void offset_origin(const Position& pos) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_offset_origin"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = wrap_Position(pos);
            
            func->Call(object, 1, args);
        }

        program_pos = program_pos + origin_pos - pos;
        origin_pos = pos;
	}
	
	virtual void units(Units u) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_units"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(u));
            
            func->Call(object, 1, args);
        }

        if (u == Units::Imperial)
        {
            if (_length_unit_type == Units::Metric)
            {
                _length_unit_type = Units::Imperial;
                origin_pos.x = (origin_pos.x / 25.4);
                origin_pos.y = (origin_pos.y / 25.4);
                origin_pos.z = (origin_pos.z / 25.4);
                program_pos.x = (program_pos.x / 25.4);
                program_pos.y = (program_pos.y / 25.4);
                program_pos.z = (program_pos.z / 25.4);
            }
        }
        else if (u == Units::Metric)
        {
            if (_length_unit_type == Units::Imperial)
            {
                _length_unit_type = Units::Metric;
                origin_pos.x = (origin_pos.x * 25.4);
                origin_pos.y = (origin_pos.y * 25.4);
                origin_pos.z = (origin_pos.z * 25.4);
                program_pos.x = (program_pos.x * 25.4);
                program_pos.y = (program_pos.y * 25.4);
                program_pos.z = (program_pos.z * 25.4);
            }
        }
	}
	virtual Units units() const override
	{
	    return _length_unit_type;
	}
	
	virtual void plane(Plane pl) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_plane"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(pl));
            
            func->Call(object, 1, args);
        }
        
        _active_plane = pl;
	}
	virtual Plane plane() const override
	{
	    return _active_plane;
	}
	
	virtual void rapid_rate(double rate) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_rapid_rate"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(rate);
            
            func->Call(object, 1, args);
        }
        
        _traverse_rate = rate;
	}
	virtual double rapid_rate() const override
	{
	    return _traverse_rate;
	}
	
	virtual void feed_rate(double rate) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_feed_rate"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(rate);
            
            func->Call(object, 1, args);
        }
        
        _feed_rate = rate;
	}
	virtual double feed_rate() const override
	{
	    return _feed_rate;
	}
	virtual void feed_reference(FeedReference reference) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_feed_reference"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(reference));
            
            func->Call(object, 1, args);
        }
	}
	
	virtual void motion_mode(Motion mode) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_motion_mode"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(mode));
            
            func->Call(object, 1, args);
        }
        
        _motion_mode = mode;
	}
	virtual Motion motion_mode() const override
	{
	    return _motion_mode;
	}
	
	virtual void cutter_radius_comp(double radius) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_cutter_radius_comp"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(radius);
            
            func->Call(object, 1, args);
        }
	}
	virtual void cutter_radius_comp_start(Side direction) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_cutter_radius_comp_start"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(direction));
            
            func->Call(object, 1, args);
        }
	}
	virtual void cutter_radius_comp_stop() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_cutter_radius_comp_stop"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	
	virtual void speed_feed_sync_start() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_speed_feed_sync_start"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual void speed_feed_sync_stop() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_speed_feed_sync_stop"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	
	virtual void rapid(const Position& pos) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_rapid"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = wrap_Position(pos);
            
            func->Call(object, 1, args);
        }

        program_pos = pos;
	}
	virtual void arc(double end0, double end1, double axis0, double axis1, int rotation, double end_point, double a, double b, double c) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_arc"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[9];
            args[0] = Number::New(end0);
            args[1] = Number::New(end1);
            args[2] = Number::New(axis0);
            args[3] = Number::New(axis1);
            args[4] = Number::New(rotation);
            args[5] = Number::New(end_point);
            args[6] = Number::New(a);
            args[7] = Number::New(b);
            args[8] = Number::New(c);
            
            func->Call(object, 9, args);
        }

        switch (_active_plane)
        {
            case Plane::XY:
            {
                program_pos.x = end0;
                program_pos.y = end1;
                program_pos.z = end_point;
                break;
            }
            case Plane::YZ:
            {
                program_pos.x = end_point;
                program_pos.y = end0;
                program_pos.z = end1;
                break;
            }
            case Plane::XZ:
            {
                program_pos.x = end1;
                program_pos.y = end_point;
                program_pos.z = end0;
                break;
            }
        }
        program_pos.a = a;
        program_pos.b = b;
        program_pos.c = c;
	}
	virtual void linear(const Position& pos) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_linear"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = wrap_Position(pos);
            
            func->Call(object, 1, args);
        }

        program_pos = pos;
	}
	virtual void probe(const Position& pos) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_probe"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = wrap_Position(pos);
            
            func->Call(object, 1, args);
        }

        double distance;
        double dx, dy, dz;
        double backoff;

        dx = (program_pos.x - pos.x);
        dy = (program_pos.y - pos.y);
        dz = (program_pos.z - pos.z);
        distance = sqrt((dx * dx) + (dy * dy) + (dz * dz));

        probe_pos = pos;
        if (distance != 0)
        {
            backoff = ((_length_unit_type == Units::Metric) ? 0.254 : 0.01);
            program_pos.x = (pos.x + (backoff * (dx / distance)));
            program_pos.y = (pos.y + (backoff * (dy / distance)));
            program_pos.z = (pos.z + (backoff * (dz / distance)));
        }
        program_pos.a = pos.a;
        program_pos.b = pos.b;
        program_pos.c = pos.c;
	}
	virtual void dwell(double seconds) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_dwell"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(seconds);
            
            func->Call(object, 1, args);
        }
	}

	virtual void spindle_start_clockwise() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_spindle_start_clockwise"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _spindle_turning = ((_spindle_speed == 0) ? Direction::Stop : Direction::Clockwise);
	}
	virtual void spindle_start_counterclockwise() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_spindle_start_counterclockwise"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _spindle_turning = ((_spindle_speed == 0) ? Direction::Stop : Direction::CounterClockwise);
	}
	virtual void spindle_stop() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_spindle_stop"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _spindle_turning = Direction::Stop;
	}
	virtual Direction spindle_state() const override
	{
	    return _spindle_turning;
	}
	virtual void spindle_speed(double r) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_spindle_speed"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(r);
            
            func->Call(object, 1, args);
        }
        
        _spindle_speed = r;
	}
	virtual double spindle_speed() const override
	{
	    return _spindle_speed;
	}
	virtual void spindle_orient(double orientation, Direction direction) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_spindle_orient"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[2];
            args[0] = Number::New(orientation);
            args[1] = Number::New(static_cast<int>(direction));
            
            func->Call(object, 2, args);
        }
	}

	virtual void tool_length_offset(double length) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_tool_length_offset"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(length);
            
            func->Call(object, 1, args);
        }
	}
	virtual void tool_change(int slot) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_tool_change"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(slot);
            
            func->Call(object, 1, args);
        }
        
        _active_slot = slot;
	}
	virtual void tool_select(int i) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_tool_select"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(i);
            
            func->Call(object, 1, args);
        }
	}
	virtual int tool_slot() const override
	{
	    return _active_slot;
	}
	virtual Tool tool(int pocket) const override
	{
	    Tool t;
    	HandleScope handle_scope;

        auto fn = object->Get("get_tool"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(pocket);
            
            auto result = func->Call(object, 1, args);
            auto obj = result->ToObject();
            if(!obj.IsEmpty())
            {
				t.id = js::to_int32(obj->Get("id"_sym));
				t.length = js::to_double(obj->Get("length"_sym));
				t.diameter = js::to_double(obj->Get("diameter"_sym));
            }
        }
        
        return t;
	}
	virtual unsigned int tool_max() const override
	{
	    unsigned int max = 1;
    	HandleScope handle_scope;

        auto fn = object->Get("get_tool_max"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            
            auto result = func->Call(object, 0, args);
            max = js::to_uint32(result);
        }
        
        return max;
	}

	virtual void axis_clamp(Axis axis) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_axis_clamp"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(axis));
            
            func->Call(object, 1, args);
        }
	}
	virtual void axis_unclamp(Axis axis) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_axis_unclamp"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = Number::New(static_cast<int>(axis));
            
            func->Call(object, 1, args);
        }
	}

	virtual void comment(const char *s) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_comment"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = String::New(s);
            
            func->Call(object, 1, args);
        }
	}

	virtual void feed_override_disable() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_feed_override_disable"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual void feed_override_enable() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_feed_override_enable"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}

	virtual void speed_override_disable() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_speed_override_disable"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual void speed_override_enable() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_speed_override_enable"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}

	virtual void coolant_flood_off() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_coolant_flood_off"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _flood = 0;
	}
	virtual void coolant_flood_on() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_coolant_flood_on"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _flood = 1;
	}
	virtual bool coolant_flood() const override
	{
	    return _flood;
	}
	
	virtual void coolant_mist_off() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_coolant_mist_off"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _mist = 0;
	}
	virtual void coolant_mist_on() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_coolant_mist_on"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
        
        _mist = 1;
	}
	virtual bool coolant_mist() const override
	{
	    return _mist;
	}

	virtual void message(const char *s) override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_message"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[1];
            args[0] = String::New(s);
            
            func->Call(object, 1, args);
        }
	}

	virtual void pallet_shuttle() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_pallet_shuttle"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}

	virtual void probe_off() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_probe_off"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual void probe_on() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_probe_on"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual Position probe_position() const override
	{
	    return probe_pos;
	}
	virtual double probe_value() const override
	{
	    double val = 1.0;
    	HandleScope handle_scope;

        auto fn = object->Get("get_probe_value"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            
            auto result = func->Call(object, 0, args);
            val = js::to_double(result);
        }
        
        return val;
	}

	virtual void program_optional_stop() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_program_optional_stop"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual void program_end() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_program_end"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}
	virtual void program_stop() override
	{
    	HandleScope handle_scope;

        auto fn = object->Get("on_program_stop"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            func->Call(object, 0, args);
        }
	}

	virtual void get_parameter_filename(char* filename, size_t max_size) const override
	{
        filename[0] = 0;

        HandleScope handle_scope;

        auto fn = object->Get("get_parameter_filename"_sym);
        if(fn->IsFunction())
        {
            Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(fn);

            Handle<Value> args[0];
            auto result = func->Call(object, 0, args);
            auto file = js::to_string(result);
            if (file.size() < max_size)
                strcpy(filename, file.c_str());
        }
	}
	virtual Position current_position() const override
	{
	    return program_pos;
	}
	virtual bool queue_empty() const override
	{
	    return true;
	}

public:
	js_rs274ngc(Handle<Object> object)
     : object(Persistent<Object>::New(object))
	{
	}
	
	virtual ~js_rs274ngc()
    {
        object.Dispose();
        object.Clear();
    }
    
    static Handle<Value> units(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(static_cast<int>(self->units()));
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> plane(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(static_cast<int>(self->plane()));
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> rapid_rate(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(self->rapid_rate());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> feed_rate(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(self->feed_rate());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> motion_mode(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(static_cast<int>(self->motion_mode()));
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> spindle_state(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(static_cast<int>(self->spindle_state()));
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> spindle_speed(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(self->spindle_speed());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> tool_slot(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Number::New(self->tool_slot());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> coolant_flood(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Boolean::New(self->coolant_flood());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> coolant_mist(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Boolean::New(self->coolant_mist());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> probe_position(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = wrap_Position(self->probe_position());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> current_position(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = wrap_Position(self->current_position());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }

    static Handle<Value> queue_empty(const Arguments& args)
    {
	    HandleScope handle_scope;
	
	    try
	    {
		    auto self = js::unwrap<js_rs274ngc>(args);
		
		    auto result = Boolean::New(self->queue_empty());
		    return handle_scope.Close(result);
	    }
	    catch(const js::error& ex)
	    {
		    return ThrowException(Exception::Error(String::New(ex.what())));
	    }
	    catch(const std::exception& ex)
	    {
		    return ThrowException(String::New(ex.what()));
	    }
    }
};

Handle<Value> init(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		self->init();
		return {};
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> synch(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		self->synch();
		return {};
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> read(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		auto command = js::to_string(args[0]);
		auto result = self->read(command.c_str());
		return Number::New(result);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> execute(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		auto result = self->execute();
		return Number::New(result);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> reset(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		self->reset();
		return {};
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> exit(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		self->exit();
		return {};
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> load_tool_table(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		self->load_tool_table();
		return {};
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> active_g_codes(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		int codes[RS274NGC_ACTIVE_G_CODES];
		self->active_g_codes(codes);
		
        auto jscodes = Array::New(RS274NGC_ACTIVE_G_CODES);
        for(std::size_t i = 0; i < RS274NGC_ACTIVE_G_CODES; ++i)
            jscodes->Set(i, Number::New(codes[i]));
		
		return handle_scope.Close(jscodes);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> active_m_codes(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		int codes[RS274NGC_ACTIVE_M_CODES];
		self->active_m_codes(codes);
		
        auto jscodes = Array::New(RS274NGC_ACTIVE_M_CODES);
        for(std::size_t i = 0; i < RS274NGC_ACTIVE_M_CODES; ++i)
            jscodes->Set(i, Number::New(codes[i]));
		
		return handle_scope.Close(jscodes);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> active_settings(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		double settings[RS274NGC_ACTIVE_SETTINGS];
		self->active_settings(settings);
		
        auto jssettings = Array::New(RS274NGC_ACTIVE_SETTINGS);
        for(std::size_t i = 0; i < RS274NGC_ACTIVE_SETTINGS; ++i)
            jssettings->Set(i, Number::New(settings[i]));
		
		return handle_scope.Close(jssettings);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> line_text(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<js_rs274ngc>(args);
		
		auto len = self->line_length();
		std::vector<char> line(len);
		self->line_text(line.data(), len);
		auto result = String::New(line.data(), line.size());
		
		return handle_scope.Close(result);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
}

Handle<Value> rs274ngc_ctor(const Arguments& args)
{
	if (!args.IsConstructCall())
		return ThrowException(String::New("Cannot call constructor as function"));

	HandleScope scope;

	try
	{
		auto rs274 = std::unique_ptr<js_rs274ngc>(new js_rs274ngc{args.This()});
		js::wrap_object<js_rs274ngc>(args.This(), rs274.get());
        rs274.release();
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}

	return args.This();
}

void bind(v8::Handle<v8::Object> global)
{
	auto name = "rs274ngc"_sym;
	
	auto tpl = FunctionTemplate::New(rs274ngc_ctor);
	
	tpl->SetClassName(name);
	auto instance_template = tpl->InstanceTemplate();
	auto prototype = tpl->PrototypeTemplate();
	
	instance_template->SetInternalFieldCount(1);

    prototype->Set("units"_sym, FunctionTemplate::New(js_rs274ngc::units)->GetFunction());
    prototype->Set("plane"_sym, FunctionTemplate::New(js_rs274ngc::plane)->GetFunction());
    prototype->Set("rapid_rate"_sym, FunctionTemplate::New(js_rs274ngc::rapid_rate)->GetFunction());
    prototype->Set("feed_rate"_sym, FunctionTemplate::New(js_rs274ngc::feed_rate)->GetFunction());
    prototype->Set("motion_mode"_sym, FunctionTemplate::New(js_rs274ngc::motion_mode)->GetFunction());
    prototype->Set("spindle_state"_sym, FunctionTemplate::New(js_rs274ngc::spindle_state)->GetFunction());
    prototype->Set("spindle_speed"_sym, FunctionTemplate::New(js_rs274ngc::spindle_speed)->GetFunction());
    prototype->Set("tool_slot"_sym, FunctionTemplate::New(js_rs274ngc::tool_slot)->GetFunction());
    prototype->Set("coolant_flood"_sym, FunctionTemplate::New(js_rs274ngc::coolant_flood)->GetFunction());
    prototype->Set("coolant_mist"_sym, FunctionTemplate::New(js_rs274ngc::coolant_mist)->GetFunction());
    prototype->Set("probe_position"_sym, FunctionTemplate::New(js_rs274ngc::probe_position)->GetFunction());
    prototype->Set("current_position"_sym, FunctionTemplate::New(js_rs274ngc::current_position)->GetFunction());
    prototype->Set("queue_empty"_sym, FunctionTemplate::New(js_rs274ngc::queue_empty)->GetFunction());

	prototype->Set("init"_sym, FunctionTemplate::New(init)->GetFunction());
	prototype->Set("synch"_sym, FunctionTemplate::New(synch)->GetFunction());
	prototype->Set("read"_sym, FunctionTemplate::New(read)->GetFunction());
	prototype->Set("execute"_sym, FunctionTemplate::New(execute)->GetFunction());
	prototype->Set("reset"_sym, FunctionTemplate::New(reset)->GetFunction());
	prototype->Set("exit"_sym, FunctionTemplate::New(exit)->GetFunction());
	prototype->Set("load_tool_table"_sym, FunctionTemplate::New(load_tool_table)->GetFunction());
	prototype->Set("active_g_codes"_sym, FunctionTemplate::New(active_g_codes)->GetFunction());
	prototype->Set("active_m_codes"_sym, FunctionTemplate::New(active_m_codes)->GetFunction());
	prototype->Set("active_settings"_sym, FunctionTemplate::New(active_settings)->GetFunction());
	prototype->Set("line_text"_sym, FunctionTemplate::New(line_text)->GetFunction());

	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
	
	//global->Set("intersects"_sym, FunctionTemplate::New(intersects)->GetFunction());
}

}

