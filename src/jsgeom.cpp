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
 * jsgeom.cpp
 *
 *  Created on: 2013-07-24
 *      Author: nicholas
 */

#include "jsgeom.h"
#include "js.h"
#include <memory>
#include "geom/polyhedron.h"
#include "geom/explode.h"
#include "geom/io.h"
#include "geom/ops.h"
#include "geom/primitives.h"
#include "geom/translate.h"
#include <fstream>

using namespace v8;
using namespace geom;

using js::operator"" _sym;

namespace jsgeom
{

Handle<Value> intersection(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO determine correct way to return new Polyhedron object.
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		std::unique_ptr<polyhedron_t> Px{new polyhedron_t};
		*Px = (*self) * (*poly);
		//js::wrap_object<polyhedron_t>(args.This(), Px.release());
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> union_(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO determine correct way to return new Polyhedron object.
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		std::unique_ptr<polyhedron_t> Px{new polyhedron_t};
		*Px = (*self) + (*poly);
		//js::wrap_object<polyhedron_t>(args.This(), Px.release());
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> difference(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO determine correct way to return new Polyhedron object.
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		std::unique_ptr<polyhedron_t> Px{new polyhedron_t};
		*Px = (*self) - (*poly);
		//js::wrap_object<polyhedron_t>(args.This(), Px.release());
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> symmetric_difference(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO determine correct way to return new Polyhedron object.
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		std::unique_ptr<polyhedron_t> Px{new polyhedron_t};
		*Px = (*self) ^ (*poly);
		//js::wrap_object<polyhedron_t>(args.This(), Px.release());
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> complement(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO determine correct way to return new Polyhedron object.
		std::unique_ptr<polyhedron_t> Px{new polyhedron_t};
		*Px = ! (*self);
		//js::wrap_object<polyhedron_t>(args.This(), Px.release());
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> empty(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		return Boolean::New(self->empty());
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> explode(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		auto parts = explode(*self);
		// TODO determine correct way to return new Polyhedron object.
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> to_object(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	auto obj = Object::New();
	try
	{
		const auto poly_object = to_object(*self);
	
		auto vertices = Array::New(poly_object.vertices.size());
		for(std::size_t v = 0; v != poly_object.vertices.size(); ++v)
		{
			auto& vtx = poly_object.vertices[v];
			auto vertex = Array::New(3);
		
			vertex->Set(0, Number::New(vtx.x));
			vertex->Set(1, Number::New(vtx.y));
			vertex->Set(2, Number::New(vtx.z));
		
			vertices->Set(v, vertex);
		}
		obj->Set("vertices"_sym, vertices);
	
		auto faces = Array::New(poly_object.faces.size());
		for(std::size_t f = 0; f != poly_object.faces.size(); ++f)
		{
			auto& fce = poly_object.faces[f];
			auto face = Array::New(fce.size());
		
			for(std::size_t f = 0; f != poly_object.faces.size(); ++f)
				face->Set(f, Number::New(fce[f]));
		
			faces->Set(f, face);
		}
		obj->Set("faces"_sym, faces);
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	
	return handle_scope.Close(obj);
}
Handle<Value> write_off(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		auto filename = js::to_string(args[0]);
		std::ofstream os(filename);
		write_off(os, *self);
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
	return {};
}
Handle<Value> glide(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO action
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> volume(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		return Number::New(volume(*self));
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> rotate(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO action
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> translate(const Arguments& args)
{
	HandleScope handle_scope;
	auto self = js::unwrap<polyhedron_t>(args);
	
	try
	{
		// TODO action
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}

Handle<Value> polyhedron_ctor(const Arguments& args)
{
	if (!args.IsConstructCall())
		return ThrowException(String::New("Cannot call constructor as function"));

	HandleScope scope;

	try
	{
		std::unique_ptr<polyhedron_t> P{new polyhedron_t};
		js::wrap_object<polyhedron_t>(args.This(), P.release());
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

Handle<Value> make_sphere(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	
	return {};
}
Handle<Value> make_box(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	
	return {};
}
Handle<Value> make_cone(const Arguments& args)
{
	HandleScope scope;

	try
	{
	}
	catch(const js::error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}

	return {};
}

void bind(v8::Handle<v8::Object> global)
{
	auto name = "Polyhedron"_sym;
	
	auto tpl = FunctionTemplate::New(polyhedron_ctor);
	
	tpl->SetClassName(name);
	auto instance_template = tpl->InstanceTemplate();
	auto prototype = tpl->PrototypeTemplate();
	
	instance_template->SetInternalFieldCount(1);

	prototype->Set("intersection"_sym, FunctionTemplate::New(intersection)->GetFunction());
	prototype->Set("union"_sym, FunctionTemplate::New(union_)->GetFunction());
	prototype->Set("difference"_sym, FunctionTemplate::New(difference)->GetFunction());
	prototype->Set("symmetric_difference"_sym, FunctionTemplate::New(symmetric_difference)->GetFunction());
	prototype->Set("complement"_sym, FunctionTemplate::New(complement)->GetFunction());
	prototype->Set("empty"_sym, FunctionTemplate::New(empty)->GetFunction());
	prototype->Set("explode"_sym, FunctionTemplate::New(explode)->GetFunction());
	prototype->Set("to_object"_sym, FunctionTemplate::New(to_object)->GetFunction());
	prototype->Set("write_off"_sym, FunctionTemplate::New(write_off)->GetFunction());
	prototype->Set("glide"_sym, FunctionTemplate::New(glide)->GetFunction());
	prototype->Set("volume"_sym, FunctionTemplate::New(volume)->GetFunction());
	prototype->Set("rotate"_sym, FunctionTemplate::New(rotate)->GetFunction());
	prototype->Set("translate"_sym, FunctionTemplate::New(translate)->GetFunction());
	prototype->Set(""_sym, FunctionTemplate::New()->GetFunction());

	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
	
	global->Set("make_sphere"_sym, FunctionTemplate::New(make_sphere)->GetFunction());
	global->Set("make_box"_sym, FunctionTemplate::New(make_box)->GetFunction());
	global->Set("make_cone"_sym, FunctionTemplate::New(make_cone)->GetFunction());
}

}

