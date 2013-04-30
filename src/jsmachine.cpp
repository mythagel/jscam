/*
 * jsmachine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "jsmachine.h"
#include <memory>
#include "Machine.h"
#include "js.h"

using namespace v8;

namespace jscam
{

std::shared_ptr<Machine> machine;

Handle<Value> init(const Arguments& args)
{
	HandleScope handle_scope;
	std::shared_ptr<Machine> machine;

	auto config = args[0]->ToObject();
	auto variant = js::to_string(args[1]);

	auto type = js::to_string(config->Get(String::NewSymbol("type")));

	if(type == "mill")
		machine = std::make_shared<Machine>(Machine::Type::Mill, variant);
	else if(type == "lathe")
		machine = std::make_shared<Machine>(Machine::Type::Lathe, variant);
	else
		return ThrowException(String::New("type - mill / lathe"));

	auto tools = config->Get(String::NewSymbol("tools"));
	for(auto t : js::array(tools))
	{
		Tool tool;
		auto tool_obj = t->ToObject();

		auto name = js::to_string(tool_obj->Get(String::NewSymbol("name")));
		auto type = js::to_string(tool_obj->Get(String::NewSymbol("type")));

		if(type == "mill")
			tool = Tool(name, Tool::type_Mill);
		else if(type == "lathe")
			tool = Tool(name, Tool::type_Lathe);

		// TODO set params (needs work in cxxcam)
		auto id = js::to_int32(tool_obj->Get(String::NewSymbol("id")));
		auto center_cutting = tool_obj->Get(String::NewSymbol("center_cutting"));
		auto flutes = js::to_uint32(tool_obj->Get(String::NewSymbol("flutes")));
		auto flute_length = js::to_double(tool_obj->Get(String::NewSymbol("flute_length")));
		auto cutting_length = js::to_double(tool_obj->Get(String::NewSymbol("cutting_length")));
		auto mill_diameter = js::to_double(tool_obj->Get(String::NewSymbol("mill_diameter")));
		auto shank_diameter = js::to_double(tool_obj->Get(String::NewSymbol("shank_diameter")));
		auto core_diameter = js::to_double(tool_obj->Get(String::NewSymbol("core_diameter")));
		auto length = js::to_double(tool_obj->Get(String::NewSymbol("length")));

		machine->AddTool(id, tool);
	}

	auto spindle_speeds = config->Get(String::NewSymbol("spindle"));
	for(auto s : js::array(spindle_speeds))
	{
		if(s->IsNumber())
		{
			auto speed = js::to_uint32(s);
			machine->AddSpindleDiscrete(speed);
		}
		else
		{
			auto speed = js::to_string(s);
			auto dash_pos = speed.find('-');
			if(dash_pos != std::string::npos)
			{
				auto start = speed.substr(0, dash_pos);
				auto end = speed.substr(dash_pos+1);
				machine->AddSpindleRange(std::stoul(start), std::stoul(end));
			}
			else
			{
				return ThrowException(String::New("range: 'start-end'"));
			}
		}
	}

	jscam::machine = machine;
	return {};
}

Handle<Value> exact_path(const Arguments&)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	machine->AccuracyExactPath();
	return {};
}
Handle<Value> exact_stop(const Arguments&)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	machine->AccuracyExactStop();
	return {};
}
Handle<Value> path_blend(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

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

	return ThrowException(String::New("expected path_blend(void / double p / double p, double q)"));
}

Handle<Value> motion(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	auto motion = js::to_string(args[0]);
	if(motion == "absolute")
	{
		machine->SetMotion(Machine::Motion::Absolute);
		return {};
	}
	else if(motion == "incremental")
	{
		machine->SetMotion(Machine::Motion::Incremental);
		return {};
	}

	return ThrowException(String::New("expected motion(absolute / incremental)"));
}
Handle<Value> arc_motion(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> units(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> plane(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> feed_rate_mode(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> feed_rate(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	if(args.Length() == 1)
	{
		auto f = js::to_double(args[0]);
		machine->SetFeedRate(f);
		return {};
	}

	return ThrowException(String::New("expected feed_rate(double feedrate)"));
}

Handle<Value> spindle_on(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	if(args.Length() == 1)
	{
		auto s = js::to_uint32(args[0]);
		machine->StartSpindle(s);
		return {};
	}
	else if(args.Length() == 2)
	{
		auto s = js::to_uint32(args[0]);
		auto r = js::to_string(args[1]);
		if(r == "stop")
		{
			machine->StartSpindle(s, Machine::Rotation::Stop);
			return {};
		}
		else if(r == "clockwise")
		{
			machine->StartSpindle(s, Machine::Rotation::Clockwise);
			return {};
		}
		else if(r == "counterclockwise")
		{
			machine->StartSpindle(s, Machine::Rotation::CounterClockwise);
			return {};
		}
	}

	return ThrowException(String::New("expected spindle_on(uint s, r = machine.Rotation.Clockwise)"));
}
Handle<Value> spindle_off(const Arguments&)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	machine->StopSpindle();
	return {};
}

Handle<Value> load_tool(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	if(args.Length() == 1)
	{
		auto id = js::to_uint32(args[0]);
		machine->SetTool(id);
		return {};
	}

	return ThrowException(String::New("expected load_tool(int id)"));
}
Handle<Value> tool_change(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	if(args.Length() == 1)
	{
		auto id = js::to_uint32(args[0]);
		machine->ToolChange(id);
		return {};
	}

	return ThrowException(String::New("expected tool_change(int id)"));
}

Handle<Value> begin_block(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	if(args.Length() == 1)
	{
		auto name = js::to_string(args[0]);
		machine->NewBlock(name);
		return {};
	}

	return ThrowException(String::New("expected begin_block(string name)"));
}
Handle<Value> end_block(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	if(args.Length() == 0)
	{
		machine->EndBlock();
		return {};
	}
	else if(args.Length() == 1)
	{
		// TODO
	}

	return ThrowException(String::New("expected end_block(void / restore)"));
}

Handle<Value> optional_pause(const Arguments& args)
{
	HandleScope handle_scope;
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

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

	return ThrowException(String::New("expected optional_pause(string comment)"));
}
Handle<Value> rapid(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> linear(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> arc(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}
Handle<Value> plunge(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	// TODO
	return {};
}

void bind(Handle<ObjectTemplate> global)
{
	using namespace v8;

	global->Set(String::New("init"), FunctionTemplate::New(init));
	global->Set(String::New("exact_path"), FunctionTemplate::New(exact_path));
	global->Set(String::New("exact_stop"), FunctionTemplate::New(exact_stop));
	global->Set(String::New("path_blend"), FunctionTemplate::New(path_blend));
	global->Set(String::New("motion"), FunctionTemplate::New(motion));
	global->Set(String::New("arc_motion"), FunctionTemplate::New(arc_motion));
	global->Set(String::New("units"), FunctionTemplate::New(units));
	global->Set(String::New("plane"), FunctionTemplate::New(plane));
	global->Set(String::New("feed_rate_mode"), FunctionTemplate::New(feed_rate_mode));
	global->Set(String::New("feed_rate"), FunctionTemplate::New(feed_rate));
	global->Set(String::New("spindle_on"), FunctionTemplate::New(spindle_on));
	global->Set(String::New("spindle_off"), FunctionTemplate::New(spindle_off));
	global->Set(String::New("load_tool"), FunctionTemplate::New(load_tool));
	global->Set(String::New("tool_change"), FunctionTemplate::New(tool_change));
	global->Set(String::New("begin_block"), FunctionTemplate::New(begin_block));
	global->Set(String::New("end_block"), FunctionTemplate::New(end_block));
	global->Set(String::New("optional_pause"), FunctionTemplate::New(optional_pause));
	global->Set(String::New("rapid"), FunctionTemplate::New(rapid));
	global->Set(String::New("linear"), FunctionTemplate::New(linear));
	global->Set(String::New("arc"), FunctionTemplate::New(arc));
	global->Set(String::New("plunge"), FunctionTemplate::New(plunge));
}

}
