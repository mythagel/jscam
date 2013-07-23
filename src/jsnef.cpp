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
 * jsnef.cpp
 *
 *  Created on: 2013-07-24
 *      Author: nicholas
 */

#include "jsnef.h"
#include "js.h"
#include "nef/polyhedron.h"

using namespace v8;
using namespace nef;

using js::operator"" _sym;

namespace jsnef
{

Handle<Value> polyhedron_ctor(const Arguments& args)
{
	if (!args.IsConstructCall())
		return ThrowException(String::New("Cannot call constructor as function"));

	HandleScope scope;

	try
	{
//		auto machine = machine_config.Construct();
//		js::wrap_object<Machine>(args.This(), machine.release());
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
	auto name = "Polyhedron"_sym;
	
	auto tpl = FunctionTemplate::New(polyhedron_ctor);
	
	tpl->SetClassName(name);
	auto instance_template = tpl->InstanceTemplate();
	auto prototype = tpl->PrototypeTemplate();
	
	instance_template->SetInternalFieldCount(1);

//	prototype->Set("push_state"_sym, FunctionTemplate::New(push_state)->GetFunction());
//	instance_template->SetAccessor("coordinate_system"_sym, coordinate_system, coordinate_system);
	
	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
}

}

