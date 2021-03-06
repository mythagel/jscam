/* jscam - C++ CAD/CAM driver library.
 * Copyright (C) 2013  Nicholas Gill
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
 * jspath.cpp
 *
 *  Created on: 2014-12-15
 *      Author: nicholas
 */

#include "jspath.h"
#include "js.h"
#include <memory>
#include "Path.h"
#include <fstream>

using namespace v8;
using namespace cxxcam;

using js::operator"" _sym;

namespace jspath
{

Position Position_js(Handle<Object> o)
{
    Position pos;
    pos.X = units::length{js::to_double(o->Get("x"_sym)) * units::millimeters};
    pos.Y = units::length{js::to_double(o->Get("y"_sym)) * units::millimeters};
    pos.Z = units::length{js::to_double(o->Get("z"_sym)) * units::millimeters};
    pos.A = units::plane_angle{js::to_double(o->Get("a"_sym)) * units::degrees};
    pos.B = units::plane_angle{js::to_double(o->Get("b"_sym)) * units::degrees};
    pos.C = units::plane_angle{js::to_double(o->Get("c"_sym)) * units::degrees};
    return pos;
}
Position_Cartesian Position_Cartesian_js(Handle<Object> o)
{
    Position_Cartesian pos;
    pos.X = units::length{js::to_double(o->Get("x"_sym)) * units::millimeters};
    pos.Y = units::length{js::to_double(o->Get("y"_sym)) * units::millimeters};
    pos.Z = units::length{js::to_double(o->Get("z"_sym)) * units::millimeters};
    return pos;
}
math::vector_3 vector3_js(Handle<Object> o)
{
    math::vector_3 v;
    v.x = js::to_double(o->Get("x"_sym));
    v.y = js::to_double(o->Get("y"_sym));
    v.z = js::to_double(o->Get("z"_sym));
    return v;
}

static Handle<Object> wrap_step(const path::step& step)
{
    using units::length_mm;
    auto s = Object::New();

    auto pos = Object::New();
    pos->Set("x"_sym, Number::New(length_mm(step.position.x).value()));
    pos->Set("y"_sym, Number::New(length_mm(step.position.y).value()));
    pos->Set("z"_sym, Number::New(length_mm(step.position.z).value()));
    s->Set("pos"_sym, pos);

    auto quat = Object::New();
    quat->Set("x"_sym, Number::New(step.orientation.R_component_2()));
    quat->Set("y"_sym, Number::New(step.orientation.R_component_3()));
    quat->Set("z"_sym, Number::New(step.orientation.R_component_4()));
    quat->Set("a"_sym, Number::New(step.orientation.R_component_1()));
    s->Set("quat"_sym, quat);

    return s;
}
static Handle<Object> wrap_path(const path::path_t& path)
{
    using units::length_mm;
    using units::plane_angle_deg;
    auto p = Object::New();

    auto steps = Array::New(path.path.size());
    for(std::size_t s = 0; s < path.path.size(); ++s)
        steps->Set(s, wrap_step(path.path[s]));
    p->Set("path"_sym, steps);
    p->Set("length"_sym, Number::New(length_mm(path.length).value()));
    p->Set("angular_length"_sym, Number::New(plane_angle_deg(path.angular_length).value()));

    return p;
}

Handle<Value> expand_linear(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
        auto start = Position_js(args[0]->ToObject());
        auto end = Position_js(args[1]->ToObject());
        auto axes = js::to_string(args[2]);
	    limits::AvailableAxes geometry;
        {
            std::vector<Axis::Type> available;
            for(auto axis : axes)
                available.push_back(to_axis(axis));
            geometry = limits::AvailableAxes(available);
        }
        auto steps_per_mm = js::to_uint32(args[3]);

        auto path = path::expand_linear(start, end, geometry, steps_per_mm);
		auto result = wrap_path(path);
		
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

Handle<Value> expand_rotary(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
        auto start = Position_js(args[0]->ToObject());
        auto end = Position_js(args[1]->ToObject());
        auto axes = js::to_string(args[2]);
	    limits::AvailableAxes geometry;
        {
            std::vector<Axis::Type> available;
            for(auto axis : axes)
                available.push_back(to_axis(axis));
            geometry = limits::AvailableAxes(available);
        }
        auto steps_per_degree = js::to_uint32(args[3]);

        auto path = path::expand_rotary(start, end, geometry, steps_per_degree);
		auto result = wrap_path(path);
		
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

Handle<Value> expand_arc(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
        auto start = Position_js(args[0]->ToObject());
        auto end = Position_js(args[1]->ToObject());
        auto center = Position_Cartesian_js(args[2]->ToObject());
        auto clockwise = js::to_bool(args[3]);
        auto plane = vector3_js(args[4]->ToObject());
        auto turns = js::to_double(args[5]);
        auto axes = js::to_string(args[6]);
	    limits::AvailableAxes geometry;
        {
            std::vector<Axis::Type> available;
            for(auto axis : axes)
                available.push_back(to_axis(axis));
            geometry = limits::AvailableAxes(available);
        }
        auto steps_per_degree = js::to_uint32(args[3]);

        auto path = path::expand_arc(start, end, center, clockwise ? path::ArcDirection::Clockwise : path::ArcDirection::CounterClockwise, plane, turns, geometry, steps_per_degree);
		auto result = wrap_path(path);
		
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

void bind(v8::Handle<v8::Object> global)
{
	global->Set("expand_linear"_sym, FunctionTemplate::New(expand_linear)->GetFunction());
	global->Set("expand_rotary"_sym, FunctionTemplate::New(expand_rotary)->GetFunction());
	global->Set("expand_arc"_sym, FunctionTemplate::New(expand_arc)->GetFunction());
}

}

