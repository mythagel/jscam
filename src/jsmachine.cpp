/*
 * jsmachine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "jsmachine.h"
#include <memory>
#include "Machine.h"

using namespace v8;

namespace jscam
{

std::shared_ptr<Machine> machine;

Handle<Value> init(const Arguments& args)
{
	machine = std::make_shared<Machine>(Machine::Type::Mill, "LinuxCNC");
	// TODO this function has to create the global machine pointer reference.
	return {};
}

Handle<Value> exact_path(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> exact_stop(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> path_blend(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
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
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
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

	return {};
}

Handle<Value> load_tool(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> tool_change(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}

Handle<Value> begin_block(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}
Handle<Value> end_block(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
}

Handle<Value> optional_pause(const Arguments& args)
{
	if(!machine)
		return ThrowException(String::New("Machine uninitialised."));

	return {};
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
