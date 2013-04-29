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
	if(args.Length() != 2)
		return ThrowException(String::New("expected init(json config, string variant)"));

	auto config = args[0]->ToObject();
	auto variant = js::to_string(args[1]);
	auto type = js::to_string(config->Get(String::NewSymbol("type")));

	if(type == "mill")
		machine = std::make_shared<Machine>(Machine::Type::Mill, variant);
	else if(type == "lathe")
		machine = std::make_shared<Machine>(Machine::Type::Lathe, variant);
	else
		return ThrowException(String::New("variant - mill / lathe"));

	{
		//	"tools":
		//	[
		//		{
		//			"name": "tool_name",
		//			"type": "mill/lathe",
		//
		//			"center_cutting": "true/false",
		//			"flutes": "int",
		//			"flute_length": "",
		//			"cutting_length": "",
		//			"mill_diameter": "",
		//			"shank_diameter": "",
		//			"core_diameter": "",
		//			"length": ""
		//		},
		//		{...}
		//	],
		//	"spindle": "0-100,300,500-1000,3000"

		auto tools = config->Get(String::NewSymbol("tools"));
		auto spindle = config->Get(String::NewSymbol("spindle"));
	}
	// TODO this function has to create the global machine pointer reference.
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

	return {};
}
Handle<Value> arc_motion(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> units(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> plane(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> feed_rate_mode(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

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
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> spindle_off(const Arguments& args)
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
		auto id = js::toint32(args[0]);
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
		auto id = js::toint32(args[0]);
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

	return {};
}
Handle<Value> linear(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> arc(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> plunge(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

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
