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
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = (*self) * (*poly);
		
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
Handle<Value> union_(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = (*self) + (*poly);
		
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
Handle<Value> difference(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = (*self) - (*poly);
		
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
Handle<Value> symmetric_difference(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		auto poly = js::unwrap<polyhedron_t>(args[0]->ToObject());
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = (*self) ^ (*poly);
		
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
Handle<Value> complement(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = !(*self);
		
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
Handle<Value> empty(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
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
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
		auto parts = explode(*self);
		auto res = Array::New(parts.size());
		for(std::size_t p = 0; p != parts.size(); ++p)
		{
			auto part = js::jsnew("Polyhedron"_sym);
			auto Px = js::unwrap<polyhedron_t>(part);
			*Px = parts[p];
			
			res->Set(p, part);
		}
		return handle_scope.Close(res);
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
Handle<Value> to_object(const Arguments& args)
{
	HandleScope handle_scope;
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		auto obj = Object::New();
		
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
		
		return handle_scope.Close(obj);
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
Handle<Value> read_off(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
		auto filename = js::to_string(args[0]);
		std::ifstream is(filename);
		is >> geom::format::off >> *self;
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
Handle<Value> write_off(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
		auto filename = js::to_string(args[0]);
		std::ofstream os(filename);
		os << geom::format::off << *self;
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
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		polyline_t path;
		
		for(auto p : js::array(args[0]))
		{
			auto po = p->ToObject();
			auto x = po->Get("x"_sym);
			auto y = po->Get("y"_sym);
			auto z = po->Get("z"_sym);
			path.line.push_back({js::to_double(x), js::to_double(y), js::to_double(z)});
		}
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = glide(*self, path);
		
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
Handle<Value> volume(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
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
}
Handle<Value> rotate(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
		auto qo = args[0]->ToObject();
		
		auto w = qo->Get("w"_sym);
		auto x = qo->Get("x"_sym);
		auto y = qo->Get("y"_sym);
		auto z = qo->Get("z"_sym);
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = rotate(*self, js::to_double(w), js::to_double(x), js::to_double(y), js::to_double(z));
		
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
Handle<Value> translate(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto self = js::unwrap<polyhedron_t>(args);
		
		auto vo = args[0]->ToObject();
		
		auto x = vo->Get("x"_sym);
		auto y = vo->Get("y"_sym);
		auto z = vo->Get("z"_sym);
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = translate(*self, js::to_double(x), js::to_double(y), js::to_double(z));
		
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
		auto center = args[0]->ToObject();
		auto cx = center->Get("x"_sym);
		auto cy = center->Get("y"_sym);
		auto cz = center->Get("z"_sym);
		
		auto r = js::to_double(args[1]);
		auto slices = js::to_uint32(args[2]);
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = geom::make_sphere({js::to_double(cx), js::to_double(cy), js::to_double(cz)}, r, slices);
		
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
Handle<Value> make_box(const Arguments& args)
{
	HandleScope handle_scope;
	
	try
	{
		auto p1 = args[0]->ToObject();
		auto p1x = p1->Get("x"_sym);
		auto p1y = p1->Get("y"_sym);
		auto p1z = p1->Get("z"_sym);
		
		auto p2 = args[1]->ToObject();
		auto p2x = p2->Get("x"_sym);
		auto p2y = p2->Get("y"_sym);
		auto p2z = p2->Get("z"_sym);
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = geom::make_box({js::to_double(p1x), js::to_double(p1y), js::to_double(p1z)}, {js::to_double(p2x), js::to_double(p2y), js::to_double(p2z)});
		
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
Handle<Value> make_cone(const Arguments& args)
{
	HandleScope handle_scope;

	try
	{
		auto p1 = args[0]->ToObject();
		auto p1x = p1->Get("x"_sym);
		auto p1y = p1->Get("y"_sym);
		auto p1z = p1->Get("z"_sym);
		
		auto p2 = args[1]->ToObject();
		auto p2x = p2->Get("x"_sym);
		auto p2y = p2->Get("y"_sym);
		auto p2z = p2->Get("z"_sym);
		
		auto r1 = js::to_double(args[2]);
		auto r2 = js::to_double(args[3]);
		auto slices = js::to_uint32(args[4]);
		
		auto result = js::jsnew("Polyhedron"_sym);
		auto Px = js::unwrap<polyhedron_t>(result);
		*Px = geom::make_cone({js::to_double(p1x), js::to_double(p1y), js::to_double(p1z)}, {js::to_double(p2x), js::to_double(p2y), js::to_double(p2z)}, r1, r2, slices);
		
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
	prototype->Set("read_off"_sym, FunctionTemplate::New(read_off)->GetFunction());
	prototype->Set("write_off"_sym, FunctionTemplate::New(write_off)->GetFunction());
	prototype->Set("glide"_sym, FunctionTemplate::New(glide)->GetFunction());
	prototype->Set("volume"_sym, FunctionTemplate::New(volume)->GetFunction());
	prototype->Set("rotate"_sym, FunctionTemplate::New(rotate)->GetFunction());
	prototype->Set("translate"_sym, FunctionTemplate::New(translate)->GetFunction());

	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
	
	global->Set("make_sphere"_sym, FunctionTemplate::New(make_sphere)->GetFunction());
	global->Set("make_box"_sym, FunctionTemplate::New(make_box)->GetFunction());
	global->Set("make_cone"_sym, FunctionTemplate::New(make_cone)->GetFunction());
}

}

