/*
 * jsmachine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "jsmachine.h"

namespace jscam
{

v8::Handle<v8::Value> setup(const v8::Arguments& args)
{
	// TODO this function has to create the global machine pointer reference.
	return {};
}

v8::Handle<v8::Value> exact_path(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> exact_stop(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> path_blend(const v8::Arguments& args)
{
	return {};
}

v8::Handle<v8::Value> motion(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> arc_motion(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> units(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> plane(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> feed_rate_mode(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> feed_rate(const v8::Arguments& args)
{
	return {};
}

v8::Handle<v8::Value> spindle_on(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> spindle_off(const v8::Arguments& args)
{
	return {};
}

v8::Handle<v8::Value> load_tool(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> tool_change(const v8::Arguments& args)
{
	return {};
}

v8::Handle<v8::Value> begin_block(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> end_block(const v8::Arguments& args)
{
	return {};
}

v8::Handle<v8::Value> optional_pause(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> rapid(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> linear(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> arc(const v8::Arguments& args)
{
	return {};
}
v8::Handle<v8::Value> plunge(const v8::Arguments& args)
{
	return {};
}

void bind(v8::Handle<v8::ObjectTemplate> global)
{
	using namespace v8;

	global->Set(String::New("setup"), FunctionTemplate::New(setup));
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
