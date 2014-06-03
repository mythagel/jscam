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
 * jsmachine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "jsmachine.h"
#include <memory>
#include "Machine.h"
#include "Configuration.h"
#include "Error.h"
#include "Axis.h"
#include "Offset.h"
#include "Tool.h"
#include "Stock.h"
#include "geom/io.h"
#include "js.h"
#include <stdexcept>

using namespace v8;
using namespace cxxcam;

using js::operator"" _sym;

namespace jsmachine
{

Handle<Value> push_state(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->PushState();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}
Handle<Value> pop_state(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->PopState();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}
Handle<Value> discard_state(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->DiscardState();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}

Handle<Value> dump_state(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->dump();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}

Handle<Value> exact_path(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->AccuracyExactPath();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}
Handle<Value> exact_stop(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->AccuracyExactStop();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}
Handle<Value> path_blend(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 0)
		{
			machine->AccuracyPathBlending();
			return {};
		}
		else if(args.Length() == 1)
		{
			auto p = js::to_double(args[0]);
			machine->AccuracyPathBlending(p);
			return {};
		}
		else if(args.Length() == 2)
		{
			auto p = js::to_double(args[0]);
			auto q = js::to_double(args[1]);
			machine->AccuracyPathBlending(p, q);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected path_blend(void / double p / double p, double q)"));
}

Handle<Value> coordinate_system(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		switch(machine->GetCoordinateSystem())
		{
			case Machine::CoordinateSystem::Active:
				return "active"_sym;
			case Machine::CoordinateSystem::P1:
				return "p1"_sym;
			case Machine::CoordinateSystem::P2:
				return "p2"_sym;
			case Machine::CoordinateSystem::P3:
				return "p3"_sym;
			case Machine::CoordinateSystem::P4:
				return "p4"_sym;
			case Machine::CoordinateSystem::P5:
				return "p5"_sym;
			case Machine::CoordinateSystem::P6:
				return "p6"_sym;
			case Machine::CoordinateSystem::P7:
				return "p7"_sym;
			case Machine::CoordinateSystem::P8:
				return "p8"_sym;
			case Machine::CoordinateSystem::P9:
				return "p9"_sym;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	throw std::logic_error("Unknown coordinate system.");
}
void coordinate_system(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		if(value == "active"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::Active);
			return;
		}
		else if(value == "p1"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P1);
			return;
		}
		else if(value == "p2"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P2);
			return;
		}
		else if(value == "p3"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P3);
			return;
		}
		else if(value == "p4"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P4);
			return;
		}
		else if(value == "p5"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P5);
			return;
		}
		else if(value == "p6"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P6);
			return;
		}
		else if(value == "p7"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P7);
			return;
		}
		else if(value == "p8"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P8);
			return;
		}
		else if(value == "p9"_sym)
		{
			machine->SetCoordinateSystem(Machine::CoordinateSystem::P9);
			return;
		}
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
		return;
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
		return;
	}

	ThrowException(String::New("expected coordinate_system = active / p1-9"));
}

Handle<Value> motion(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		switch(machine->GetMotion())
		{
			case Machine::Motion::Absolute:
				return "absolute"_sym;
			case Machine::Motion::Incremental:
				return "incremental"_sym;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	throw std::logic_error("Unknown motion type.");
}
void motion(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		if(value == "absolute"_sym)
		{
			machine->SetMotion(Machine::Motion::Absolute);
			return;
		}
		else if(value == "incremental"_sym)
		{
			machine->SetMotion(Machine::Motion::Incremental);
			return;
		}
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
		return;
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
		return;
	}

	ThrowException(String::New("expected motion = absolute / incremental"));
}

Handle<Value> arc_motion(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		switch(machine->GetArcMotion())
		{
			case Machine::Motion::Absolute:
				return "absolute"_sym;
			case Machine::Motion::Incremental:
				return "incremental"_sym;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	throw std::logic_error("Unknown arc motion type.");
}
void arc_motion(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		if(value == "absolute"_sym)
		{
			machine->SetArcMotion(Machine::Motion::Absolute);
			return;
		}
		else if(value == "incremental"_sym)
		{
			machine->SetArcMotion(Machine::Motion::Incremental);
			return;
		}
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
		return;
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
		return;
	}

	ThrowException(String::New("expected arc_motion = absolute / incremental"));
}

Handle<Value> units(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		switch(machine->GetUnits())
		{
			case Machine::Units::Metric:
				return "metric"_sym;
			case Machine::Units::Imperial:
				return "imperial"_sym;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	throw std::logic_error("Unknown units.");
}
void units(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		if(value == "metric"_sym)
		{
			machine->SetUnits(Machine::Units::Metric);
			return;
		}
		else if(value == "imperial"_sym)
		{
			machine->SetUnits(Machine::Units::Imperial);
			return;
		}
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
		return;
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
		return;
	}

	ThrowException(String::New("expected units = metric / imperial"));
}

Handle<Value> plane(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		switch(machine->GetPlane())
		{
			case Machine::Plane::XY:
				return "XY"_sym;
			case Machine::Plane::ZX:
				return "ZX"_sym;
			case Machine::Plane::YZ:
				return "YZ"_sym;
			case Machine::Plane::UV:
				return "UV"_sym;
			case Machine::Plane::WU:
				return "WU"_sym;
			case Machine::Plane::VW:
				return "VW"_sym;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	throw std::logic_error("Unknown plane.");
}
void plane(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		if(value == "XY"_sym)
		{
			machine->SetPlane(Machine::Plane::XY);
			return;
		}
		else if(value == "ZX"_sym)
		{
			machine->SetPlane(Machine::Plane::ZX);
			return;
		}
		else if(value == "YZ"_sym)
		{
			machine->SetPlane(Machine::Plane::YZ);
			return;
		}
		else if(value == "UV"_sym)
		{
			machine->SetPlane(Machine::Plane::UV);
			return;
		}
		else if(value == "WU"_sym)
		{
			machine->SetPlane(Machine::Plane::WU);
			return;
		}
		else if(value == "VW"_sym)
		{
			machine->SetPlane(Machine::Plane::VW);
			return;
		}
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
		return;
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
		return;
	}

	ThrowException(String::New("expected plane = XY / ZX / YZ / UV / WU / VW"));
}

Handle<Value> feed_rate_mode(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		switch(machine->GetFeedRate().second)
		{
			case Machine::FeedRateMode::InverseTime:
				return "inverse"_sym;
			case Machine::FeedRateMode::UnitsPerMinute:
				return "upm"_sym;
			case Machine::FeedRateMode::UnitsPerRevolution:
				return "upr"_sym;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	throw std::logic_error("Unknown feed rate mode.");
}
void feed_rate_mode(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		if(value == "inverse"_sym)
		{
			machine->SetFeedRateMode(Machine::FeedRateMode::InverseTime);
			return;
		}
		else if(value == "upm"_sym)
		{
			machine->SetFeedRateMode(Machine::FeedRateMode::UnitsPerMinute);
			return;
		}
		else if(value == "upr"_sym)
		{
			machine->SetFeedRateMode(Machine::FeedRateMode::UnitsPerRevolution);
			return;
		}
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
		return;
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
		return;
	}

	ThrowException(String::New("expected feed_rate_mode = inverse / upm / upr"));
}

Handle<Value> feed_rate(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		return Number::New(machine->GetFeedRate().first);
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
}
void feed_rate(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	try
	{
		auto f = js::to_double(value);
		machine->SetFeedRate(f);
	}
	catch(const cxxcam::error& ex)
	{
		ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		ThrowException(Exception::Error(String::New(ex.what())));
	}
}

Handle<Value> spindle(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	auto state = Object::New();
	try
	{
		auto s = machine->GetSpindleState();
		state->Set("speed"_sym, Number::New(s.first));
		switch(s.second)
		{
			case Machine::Rotation::Stop:
				state->Set("direction"_sym, "stop"_sym);
				break;
			case Machine::Rotation::Clockwise:
				state->Set("direction"_sym, "clockwise"_sym);
				break;
			case Machine::Rotation::CounterClockwise:
				state->Set("direction"_sym, "counterclockwise"_sym);
				break;
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	return state;
}

Handle<Value> tool(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	auto tool = Object::New();
	try
	{
		auto tl = machine->GetTool();
		tool->Set("name"_sym, String::New(tl.Name().c_str(), tl.Name().size()));
		switch(tl.ToolType())
		{
			case Tool::Type::Mill:
			{
				auto spec = tl.GetMill();
				tool->Set("type"_sym, "mill"_sym);
			
				tool->Set("center_cutting"_sym, Number::New(spec.center_cutting));
				tool->Set("flutes"_sym, Number::New(spec.flutes));
				tool->Set("flute_length"_sym, Number::New(spec.flute_length));
				tool->Set("cutting_length"_sym, Number::New(spec.cutting_length));
				tool->Set("mill_diameter"_sym, Number::New(spec.mill_diameter));
				tool->Set("shank_diameter"_sym, Number::New(spec.shank_diameter));
				tool->Set("core_diameter"_sym, Number::New(spec.core_diameter));
				tool->Set("length"_sym, Number::New(spec.length));
				break;
			}
			case Tool::Type::Lathe:
			{
				//auto spec = tl.GetLathe();
				tool->Set("type"_sym, "lathe"_sym);
				// TODO fill to js
				break;
			}
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	
	return tool;
}

Handle<Value> stock(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	try
	{
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<geom::polyhedron_t>(result);
		*Px = machine->GetStock().Model;
		
		return handle_scope.Close(result);
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
}

Handle<Value> spindle_on(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 1)
		{
			auto s = js::to_uint32(args[0]);
			machine->StartSpindle(s);
			return {};
		}
		else if(args.Length() == 2)
		{
			auto s = js::to_uint32(args[0]);
			auto r = args[1];
			if(r == "stop"_sym)
			{
				machine->StartSpindle(s, Machine::Rotation::Stop);
				return {};
			}
			else if(r == "clockwise"_sym)
			{
				machine->StartSpindle(s, Machine::Rotation::Clockwise);
				return {};
			}
			else if(r == "counterclockwise"_sym)
			{
				machine->StartSpindle(s, Machine::Rotation::CounterClockwise);
				return {};
			}
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected spindle_on(uint s, r = machine.Rotation.Clockwise)"));
}
Handle<Value> spindle_off(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		machine->StopSpindle();
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}

Handle<Value> preload_tool(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 1)
		{
			auto id = js::to_uint32(args[0]);
			machine->SetTool(id);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected preload_tool(int id)"));
}
Handle<Value> tool_change(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 1)
		{
			auto id = js::to_uint32(args[0]);
			machine->ToolChange(id);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected tool_change(int id)"));
}

Handle<Value> begin_block(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 1)
		{
			auto name = js::to_string(args[0]);
			machine->NewBlock(name);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected begin_block(string name)"));
}
Handle<Value> end_block(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 0)
		{
			machine->EndBlock();
			return {};
		}
		else
		{
			int restore(0);
			for(auto arg : js::arguments(args))
			{
				auto res = arg;
				if(res == "preserve"_sym)
				{
					machine->EndBlock(Machine::block_PreserveState);
					return {};
				}
				else if(res == "restore"_sym)
				{
					machine->EndBlock(Machine::block_RestoreState);
					return {};
				}
				else if(res == "units"_sym)
					restore |= Machine::block_RestoreUnits;
				else if(res == "plane"_sym)
					restore |= Machine::block_RestorePlane;
				else if(res == "motion"_sym)
					restore |= Machine::block_RestoreMotion;
				else if(res == "arc_motion"_sym)
					restore |= Machine::block_RestoreArcMotion;
				else if(res == "feedrate_mode"_sym)
					restore |= Machine::block_RestoreFeedRateMode;
				else if(res == "feedrate"_sym)
					restore |= Machine::block_RestoreFeedRate;
				else if(res == "spindle"_sym)
					restore |= Machine::block_RestoreSpindle;
				else if(res == "tool"_sym)
					restore |= Machine::block_RestoreTool;
				else if(res == "position"_sym)
					restore |= Machine::block_RestorePosition;
			}
			machine->EndBlock(restore);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected end_block(void / restore)"));
}

Handle<Value> optional_pause(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		if(args.Length() == 0)
		{
			machine->OptionalPause();
			return {};
		}
		else if(args.Length() == 1)
		{
			auto comment = js::to_string(args[0]);
			machine->OptionalPause(comment);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected optional_pause(string comment)"));
}

Handle<Value> comment(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		if(args.Length() == 1)
		{
			auto comment = js::to_string(args[0]);
			machine->Comment(comment);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected comment(string comment)"));
}

Handle<Value> dwell(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	try
	{
		if(args.Length() == 1)
		{
			auto seconds = js::to_double(args[0]);
			machine->Dwell(seconds);
			return {};
		}
		else if(args.Length() == 2)
		{
			auto seconds = js::to_double(args[0]);
			auto comment = js::to_string(args[1]);
			machine->Dwell(seconds, comment);
			return {};
		}
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return ThrowException(String::New("expected comment(string comment)"));
}

Handle<Value> rapid(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		std::vector<Axis> axes;
		for(auto arg : js::arguments(args))
		{
			auto axis_value = arg->ToObject();
			auto keys = axis_value->GetPropertyNames();
			for(auto axis : js::array(keys))
			{
				auto value = axis_value->Get(axis);

				if(axis == "x"_sym)
					axes.push_back(X(js::to_double(value)));
				else if(axis == "y"_sym)
					axes.push_back(Y(js::to_double(value)));
				else if(axis == "z"_sym)
					axes.push_back(Z(js::to_double(value)));

				else if(axis == "a"_sym)
					axes.push_back(A(js::to_double(value)));
				else if(axis == "b"_sym)
					axes.push_back(B(js::to_double(value)));
				else if(axis == "c"_sym)
					axes.push_back(C(js::to_double(value)));

				else if(axis == "u"_sym)
					axes.push_back(U(js::to_double(value)));
				else if(axis == "v"_sym)
					axes.push_back(V(js::to_double(value)));
				else if(axis == "w"_sym)
					axes.push_back(W(js::to_double(value)));

				else
					return ThrowException(String::New("Unrecognised axis."));
			}
		}
		machine->Rapid(axes);
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return {};
}
Handle<Value> linear(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		std::vector<Axis> axes;
		for(auto arg : js::arguments(args))
		{
			auto axis_value = arg->ToObject();
			auto keys = axis_value->GetPropertyNames();

			for(auto axis : js::array(keys))
			{
				auto value = axis_value->Get(axis);
				if(axis == "x"_sym)
					axes.push_back(X(js::to_double(value)));
				else if(axis == "y"_sym)
					axes.push_back(Y(js::to_double(value)));
				else if(axis == "z"_sym)
					axes.push_back(Z(js::to_double(value)));

				else if(axis == "a"_sym)
					axes.push_back(A(js::to_double(value)));
				else if(axis == "b"_sym)
					axes.push_back(B(js::to_double(value)));
				else if(axis == "c"_sym)
					axes.push_back(C(js::to_double(value)));

				else if(axis == "u"_sym)
					axes.push_back(U(js::to_double(value)));
				else if(axis == "v"_sym)
					axes.push_back(V(js::to_double(value)));
				else if(axis == "w"_sym)
					axes.push_back(W(js::to_double(value)));

				else
					return ThrowException(String::New("Unrecognised axis."));
			}
		}
		machine->Linear(axes);
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return {};
}
Handle<Value> arc(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	try
	{
		Machine::Direction dir = Machine::Direction::Clockwise;
		std::vector<Axis> end_pos;
		std::vector<Offset> center;
		unsigned int turns = 1;

		/*
		 * TODO this is messy and horrible.
		 * Define a common binding strategy and stick to it.
		 */
		unsigned int index(0);
		for(auto arg : js::arguments(args))
		{
			++index;

			if(index == 1)
			{
				auto d = js::to_string(arg);
				if(d == "clockwise")
				{
					dir = Machine::Direction::Clockwise;
				}
				else if(d == "counterclockwise")
				{
					dir = Machine::Direction::CounterClockwise;
				}
				else
				{
					return ThrowException(String::New("Unrecognised direction"));
				}
			}
			else if(arg->IsObject())
			{
				auto axis_value = arg->ToObject();
				auto keys = axis_value->GetPropertyNames();

				for(auto key : js::array(keys))
				{
					auto axis = js::to_string(key);
					auto value = axis_value->Get(key);

					if(axis == "x")
						end_pos.push_back(X(js::to_double(value)));
					else if(axis == "y")
						end_pos.push_back(Y(js::to_double(value)));
					else if(axis == "z")
						end_pos.push_back(Z(js::to_double(value)));

					else if(axis == "a")
						end_pos.push_back(A(js::to_double(value)));
					else if(axis == "b")
						end_pos.push_back(B(js::to_double(value)));
					else if(axis == "c")
						end_pos.push_back(C(js::to_double(value)));

					else if(axis == "u")
						end_pos.push_back(U(js::to_double(value)));
					else if(axis == "v")
						end_pos.push_back(V(js::to_double(value)));
					else if(axis == "w")
						end_pos.push_back(W(js::to_double(value)));

					// Offsets
					else if(axis == "i")
						center.push_back(I(js::to_double(value)));
					else if(axis == "j")
						center.push_back(J(js::to_double(value)));
					else if(axis == "k")
						center.push_back(K(js::to_double(value)));

					else if(axis == "turns")
						turns = js::to_uint32(value);

					else
						return ThrowException(String::New("Unrecognised axis / offset / turns."));
				}
			}
			else
			{

			}
		}
		machine->Arc(dir, end_pos, center, turns);
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return {};
}
Handle<Value> plunge(const Arguments&)
{
	HandleScope handle_scope;
	//auto machine = js::unwrap<Machine>(args);
	
	try
	{
		// TODO missing in cxxcam
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	return {};
}
Handle<Value> generate(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	using gcode::Word;
	
	auto word2js = [](const Word& word) -> Handle<Object>
	{
		auto js_word = Object::New();
		
		auto c = to_string(word);
		js_word->Set(String::NewSymbol(c.c_str(), c.size()), Number::New(word.Value()));
		
		if(!word.Comment().empty())
			js_word->Set("comment"_sym, String::New(word.Comment().c_str(), word.Comment().size()));
		
		return js_word;
	};
	auto block2js = [word2js](const Machine::block_t& block) -> Handle<Array>
	{
		if(block.empty())
			return Array::New();
		
		auto js_block = Array::New(block.words.size() + (block.comment.empty() ? 0 : 1));

		for(std::size_t w = 0; w != block.words.size(); ++w)
			js_block->Set(w, word2js(block.words[w]));
		
		if(!block.comment.empty())
			js_block->Set(block.words.size(), String::New(block.comment.c_str(), block.comment.size()));
		
		return js_block;
	};
	
	try
	{
		auto blocks = machine->Generate();
	
		auto js_blocks = Array::New(blocks.size());
		for(std::size_t l = 0; l != blocks.size(); ++l)
			js_blocks->Set(l, block2js(blocks[l]));
		
		return handle_scope.Close(js_blocks);
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
}

Handle<Value> machine_ctor(const Arguments& args)
{
	if (!args.IsConstructCall())
		return ThrowException(String::New("Cannot call constructor as function"));

	HandleScope scope;

	try
	{
		auto config = args[0]->ToObject();
		if(config.IsEmpty())
			return ThrowException(String::New("Expected config"));

		auto stock = args[1];

		Configuration machine_config;

		{
			auto type = js::to_string(config->Get("type"_sym));
			if(type == "mill")
				machine_config.type = Machine::Type::Mill;
			else if(type == "lathe")
				machine_config.type = Machine::Type::Lathe;
			else
				return ThrowException(String::New("type - mill / lathe"));
		}
		
		{
			auto units = js::to_string(config->Get("units"_sym));
			if(!units.empty())
			{
				if(units == "metric")
					machine_config.units = Machine::Units::Metric;
				else if(units == "imperial")
					machine_config.units = Machine::Units::Imperial;
				else
					return ThrowException(String::New("units - metric / imperial"));
			}
		}
		
		{
			auto axes = js::to_string(config->Get("axes"_sym));
			if(!axes.empty())
				machine_config.axes = axes;
		}

        auto tools = config->Get("tools"_sym)->ToObject();
        if(!tools.IsEmpty())
		{
			auto tids = tools->GetPropertyNames();
			for(auto tid : js::array(tids))
			{
				Tool tool;
				auto tool_obj = tools->Get(tid)->ToObject();

				auto id = js::to_int32(tid);
				auto name = js::to_string(tool_obj->Get("name"_sym));
				auto type = js::to_string(tool_obj->Get("type"_sym));
	
				if(type == "mill")
				{
					auto spec = Tool::Mill();

					spec.type = Tool::Mill::Type::End;
					spec.center_cutting = js::to_bool(tool_obj->Get("center_cutting"_sym));
					spec.flutes = js::to_uint32(tool_obj->Get("flutes"_sym));
					spec.flute_length = js::to_double(tool_obj->Get("flute_length"_sym));
					spec.cutting_length = js::to_double(tool_obj->Get("cutting_length"_sym));
					spec.mill_diameter = js::to_double(tool_obj->Get("mill_diameter"_sym));
					spec.shank_diameter = js::to_double(tool_obj->Get("shank_diameter"_sym));
					spec.core_diameter = js::to_double(tool_obj->Get("core_diameter"_sym));
					spec.length = js::to_double(tool_obj->Get("length"_sym));
		
					tool = Tool(name, spec);
				}
				else if(type == "lathe")
				{
					// TODO fill spec from js
					auto spec = Tool::Lathe();
					tool = Tool(name, spec);
				}
				else
				{
					return ThrowException(String::New("tool type - mill / lathe"));
				}

				machine_config.tools[id] = tool;
			}
		}

		auto fr = config->Get("feed_rate"_sym);
		if(fr->IsObject())
		{
			auto feed_rate = fr->ToObject();
			auto axes = feed_rate->GetPropertyNames();
			for(auto axis : js::array(axes))
			{
				if(axis == "x"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::X] = js::to_double(feed_rate->Get(axis));
				else if(axis == "y"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::Y] = js::to_double(feed_rate->Get(axis));
				else if(axis == "z"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::Z] = js::to_double(feed_rate->Get(axis));
				
				else if(axis == "a"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::A] = js::to_double(feed_rate->Get(axis));
				else if(axis == "b"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::B] = js::to_double(feed_rate->Get(axis));
				else if(axis == "c"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::C] = js::to_double(feed_rate->Get(axis));
				
				else if(axis == "u"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::U] = js::to_double(feed_rate->Get(axis));
				else if(axis == "v"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::V] = js::to_double(feed_rate->Get(axis));
				else if(axis == "w"_sym)
					machine_config.axis_max_feed_rates[Axis::Type::W] = js::to_double(feed_rate->Get(axis));
				
				else if(axis == "max"_sym)
					machine_config.max_feed_rate = js::to_double(feed_rate->Get(axis));
				
				else
					return ThrowException(String::New("feed_rate - xyzabcuvw / max"));
			}
		}
		
		auto rr = config->Get("rapid_rate"_sym);
		if(rr->IsObject())
		{
			auto rapid_rate = config->Get("rapid_rate"_sym)->ToObject();
			auto axes = rapid_rate->GetPropertyNames();
			for(auto axis : js::array(axes))
			{
				if(axis == "x"_sym)
					machine_config.axis_rapid_rates[Axis::Type::X] = js::to_double(rapid_rate->Get(axis));
				else if(axis == "y"_sym)
					machine_config.axis_rapid_rates[Axis::Type::Y] = js::to_double(rapid_rate->Get(axis));
				else if(axis == "z"_sym)
					machine_config.axis_rapid_rates[Axis::Type::Z] = js::to_double(rapid_rate->Get(axis));
				
				else if(axis == "a"_sym)
					machine_config.axis_rapid_rates[Axis::Type::A] = js::to_double(rapid_rate->Get(axis));
				else if(axis == "b"_sym)
					machine_config.axis_rapid_rates[Axis::Type::B] = js::to_double(rapid_rate->Get(axis));
				else if(axis == "c"_sym)
					machine_config.axis_rapid_rates[Axis::Type::C] = js::to_double(rapid_rate->Get(axis));
				
				else if(axis == "u"_sym)
					machine_config.axis_rapid_rates[Axis::Type::U] = js::to_double(rapid_rate->Get(axis));
				else if(axis == "v"_sym)
					machine_config.axis_rapid_rates[Axis::Type::V] = js::to_double(rapid_rate->Get(axis));
				else if(axis == "w"_sym)
					machine_config.axis_rapid_rates[Axis::Type::W] = js::to_double(rapid_rate->Get(axis));
				
				else if(axis == "max"_sym)
					machine_config.rapid_rate = js::to_double(rapid_rate->Get(axis));
				
				else
					return ThrowException(String::New("rapid_rate - xyzabcuvw / max"));
			}
		}

		{
			auto spindle_speeds = config->Get("spindle"_sym);
			for(auto s : js::array(spindle_speeds))
			{
				auto entry = s->ToObject();
				if(entry.IsEmpty())
					return ThrowException(String::New("Expected object"));
			
				auto rpm = entry->Get("rpm"_sym);
				auto nm = entry->Get("nm"_sym);
			
				if(rpm->IsNumber())
				{
					auto speed = js::to_uint32(rpm);
					auto torque = js::to_double(nm);
					machine_config.spindle_speeds.emplace_back(speed, torque);
				}
				else
				{
					auto speed = Array::Cast(*rpm);
					auto torque = Array::Cast(*nm);
				
					auto speed_low = js::to_uint32(speed->Get(0));
					auto speed_high = js::to_uint32(speed->Get(1));
					auto torque_low = js::to_double(torque->Get(0));
					auto torque_high = js::to_double(torque->Get(1));
				
					machine_config.spindle_speeds.emplace_back(speed_low, speed_high, torque_low, torque_high);
				}
			}
		}
		
		if(stock->IsObject())
		{
			auto so = stock->ToObject();
			auto model = so->Get("model"_sym);
			if(model->IsObject())
			{
				auto poly = js::unwrap<geom::polyhedron_t>(model->ToObject());
				machine_config.stock.Model = *poly;
			}
		}
		
		auto machine = machine_config.Construct();
		js::wrap_object<Machine>(args.This(), machine.release());
	}
	catch(const cxxcam::error& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}

	return args.This();
}

void bind(Handle<Object> global)
{
	auto name = "Machine"_sym;
	
	auto tpl = FunctionTemplate::New(machine_ctor);
	
	tpl->SetClassName(name);
	auto instance_template = tpl->InstanceTemplate();
	auto prototype = tpl->PrototypeTemplate();
	
	instance_template->SetInternalFieldCount(1);

	prototype->Set("push_state"_sym, FunctionTemplate::New(push_state)->GetFunction());
	prototype->Set("pop_state"_sym, FunctionTemplate::New(pop_state)->GetFunction());
	prototype->Set("discard_state"_sym, FunctionTemplate::New(discard_state)->GetFunction());
	prototype->Set("dump_state"_sym, FunctionTemplate::New(dump_state)->GetFunction());
	
	prototype->Set("exact_path"_sym, FunctionTemplate::New(exact_path)->GetFunction());
	prototype->Set("exact_stop"_sym, FunctionTemplate::New(exact_stop)->GetFunction());
	prototype->Set("path_blend"_sym, FunctionTemplate::New(path_blend)->GetFunction());
	
	instance_template->SetAccessor("coordinate_system"_sym, coordinate_system, coordinate_system);
	instance_template->SetAccessor("motion"_sym, motion, motion);
	instance_template->SetAccessor("arc_motion"_sym, arc_motion, arc_motion);
	instance_template->SetAccessor("units"_sym, units, units);
	instance_template->SetAccessor("plane"_sym, plane, plane);
	instance_template->SetAccessor("feed_rate_mode"_sym, feed_rate_mode, feed_rate_mode);
	instance_template->SetAccessor("feed_rate"_sym, feed_rate, feed_rate);
	instance_template->SetAccessor("spindle"_sym, spindle);
	instance_template->SetAccessor("tool"_sym, tool);
	instance_template->SetAccessor("stock"_sym, stock);
	
	prototype->Set("spindle_on"_sym, FunctionTemplate::New(spindle_on)->GetFunction());
	prototype->Set("spindle_off"_sym, FunctionTemplate::New(spindle_off)->GetFunction());
	prototype->Set("preload_tool"_sym, FunctionTemplate::New(preload_tool)->GetFunction());
	prototype->Set("tool_change"_sym, FunctionTemplate::New(tool_change)->GetFunction());
	prototype->Set("begin_block"_sym, FunctionTemplate::New(begin_block)->GetFunction());
	prototype->Set("end_block"_sym, FunctionTemplate::New(end_block)->GetFunction());
	prototype->Set("optional_pause"_sym, FunctionTemplate::New(optional_pause)->GetFunction());
	prototype->Set("comment"_sym, FunctionTemplate::New(comment)->GetFunction());
	prototype->Set("dwell"_sym, FunctionTemplate::New(dwell)->GetFunction());
	prototype->Set("rapid"_sym, FunctionTemplate::New(rapid)->GetFunction());
	prototype->Set("linear"_sym, FunctionTemplate::New(linear)->GetFunction());
	prototype->Set("arc"_sym, FunctionTemplate::New(arc)->GetFunction());
	prototype->Set("plunge"_sym, FunctionTemplate::New(plunge)->GetFunction());
	prototype->Set("generate"_sym, FunctionTemplate::New(generate)->GetFunction());
	
	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
}

}
