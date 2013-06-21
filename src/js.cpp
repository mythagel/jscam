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
 * js.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "js.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <iterator>
#include "platform.h"

using namespace v8;
using platform::realpath;
using platform::dirname;

namespace js
{

namespace
{
std::ostream& operator<<(std::ostream& os, const TryCatch& try_catch)
{
	HandleScope handle_scope;

	auto message = try_catch.Message();
	auto exception = to_string(try_catch.Exception());
	
	if (message.IsEmpty())
	{
		os << "exception: " << exception << '\n';
		return os;
	}
	
	auto filename = to_string(message->GetScriptResourceName());
	int lineno = message->GetLineNumber();
	os << filename << ':' << lineno << ": " << exception << '\n';
	
	auto sourceline = to_string(message->GetSourceLine());
	os << sourceline << '\n';
	
	int start = message->GetStartColumn();
	int end = message->GetEndColumn();
	for(int i = 0; i < start; ++i)
		os << (sourceline[i] == '\t' ? '\t' : ' ');
	os << std::string(std::abs(end - start), '^') << '\n';
	
	auto stack_trace = try_catch.StackTrace();
	if(!stack_trace.IsEmpty())
		os << to_string(stack_trace) << '\n';
	
	return os;
}

}

Handle<String> read_stream(std::istream& is)
{
	if(!is)
		return {};

	auto str = std::string(std::istreambuf_iterator<char>(is.rdbuf()), std::istreambuf_iterator<char>());
	return String::New(str.c_str(), str.size());
}

std::string to_string(Handle<Value> s)
{
	if(s.IsEmpty())
		throw error("Empty Handle");
	if(s->IsUndefined())
		return {};
	auto str = s->ToString();
	if(str.IsEmpty())
		throw error("String conversion failed");
	
	String::Utf8Value value(str);
	return {*value, *value + value.length()};
}

double to_double(Handle<Value> d)
{
	if(d.IsEmpty())
		throw error("Empty Handle");
	return d->NumberValue();
}

int32_t to_int32(Handle<Value> i)
{
	if(i.IsEmpty())
		throw error("Empty Handle");
	return i->IntegerValue();
}

uint32_t to_uint32(v8::Handle<v8::Value> i)
{
	if(i.IsEmpty())
		throw error("Empty Handle");
	return i->Uint32Value();
}

bool to_bool(v8::Handle<v8::Value> b)
{
	if(b.IsEmpty())
		throw error("Empty Handle");
	return b->BooleanValue();
}

namespace detail
{
Local<Value> v8_argument_iterator_adapter::iterator::operator*()
{
	return a.args[index];
}
v8_argument_iterator_adapter::iterator& v8_argument_iterator_adapter::iterator::operator++()
{
	++index;
	return *this;
}
bool v8_argument_iterator_adapter::iterator::operator!=(const iterator& o) const
{
	return index != o.index;
}

v8_argument_iterator_adapter::iterator begin(const v8_argument_iterator_adapter& a)
{
	return {a, 0};
}
v8_argument_iterator_adapter::iterator end(const v8_argument_iterator_adapter& a)
{
	return {a, a.args.Length()};
}

Local<Value> v8_object_iterator_adapter::iterator::operator*()
{
	return a.obj->Get(index);
}
v8_object_iterator_adapter::iterator& v8_object_iterator_adapter::iterator::operator++()
{
	++index;
	return *this;
}
bool v8_object_iterator_adapter::iterator::operator!=(const iterator& o) const
{
	return index != o.index;
}

v8_object_iterator_adapter::iterator begin(const v8_object_iterator_adapter& a)
{
	return {a, 0};
}
v8_object_iterator_adapter::iterator end(const v8_object_iterator_adapter& a)
{
	if(a.obj->IsUndefined())
		return {a, 0};
	return {a, a.obj->Length()};
}

}

detail::v8_argument_iterator_adapter arguments(const Arguments& args)
{
	return { args };
}

detail::v8_object_iterator_adapter array(Local<Value> obj)
{
	return { Array::Cast(*obj) };
}

Handle<Value> print(const Arguments& args)
{
	HandleScope handle_scope;

	bool first = true;
	for(auto arg : arguments(args))
	{
		HandleScope handle_scope;
		if (first)
			first = false;
		else
			std::cout << ' ';

		String::Utf8Value str(arg);
		std::cout << *str;
	}
	std::cout << std::endl;

	return handle_scope.Close(Undefined());
}
Handle<Value> read(const Arguments& args)
{
	HandleScope handle_scope;
	
	if (args.Length() != 1)
		return ThrowException(String::New("Expected: filename"));

	String::Utf8Value file(args[0]);
	if (*file == nullptr)
		return ThrowException(String::New("Expected: string"));

	std::ifstream ifs(*file);
	auto source = read_stream(ifs);
	if (source.IsEmpty())
		return ThrowException(String::New("Unable to read file"));

	return handle_scope.Close(source);
}
Handle<Value> write(const Arguments& args)
{
	HandleScope handle_scope;
	if (args.Length() != 2)
		return ThrowException(String::New("Expected: filename, data"));

	try
	{
		auto file = to_string(args[0]);
		std::ofstream ofs(file);
		if(!ofs)
			return ThrowException(String::New("Unable to open file for writing."));
	
		auto content = to_string(args[1]);
		bool result = ofs.write(content.c_str(), content.size());
		
		return handle_scope.Close(Boolean::New(result));
	}
	catch(const error& ex)
	{
		return ThrowException(Exception::Error(String::New(ex.what())));
	}
}
Handle<Value> load(const Arguments& args)
{
	HandleScope handle_scope;
	auto context = args.Holder()->CreationContext();
	auto global = context->Global();
	auto parent_script = global->Get("__filename"_sym);
	auto parent_path = dirname(to_string(parent_script)) + '/';

	for(auto arg : arguments(args))
	{
		HandleScope handle_scope;
		try
		{
			auto file = to_string(arg);
			if(file.empty())
				continue;
			
			if(file[0] != '/')
				file = parent_path + file;
			
			std::ifstream ifs(file);
			auto source = read_stream(ifs);
			if (source.IsEmpty())
				return ThrowException(String::New("Unable to read file"));

			auto filename = realpath(file);
			if (!exec(source, String::New(filename.c_str(), filename.size()), context))
				return ThrowException(String::New("Unable to execute file"));
			
			global->Set("__filename"_sym, parent_script);
		}
		catch(const error& ex)
		{
			return ThrowException(Exception::Error(String::New(ex.what())));
		}
	}
	return handle_scope.Close(Undefined());
}

bool exec(Handle<String> source, Handle<Value> name, Handle<Context> context)
{
	HandleScope handle_scope;
	TryCatch try_catch;

	auto script = Script::Compile(source, name);
	if (script.IsEmpty())
	{
		auto exception = try_catch.Exception();
		String::AsciiValue exception_str(exception);
		std::cerr << *exception_str << std::endl;
		return false;
	}

	if(!context.IsEmpty())
	{
		auto global = context->Global();
		global->Set("__filename"_sym, name);
	}

	auto result = script->Run();
	if (result.IsEmpty())
	{
		std::cerr << try_catch << '\n';
		return false;
	}

	return true;
}

void bind(Handle<ObjectTemplate> global)
{
	global->Set(String::New("print"), FunctionTemplate::New(print));
	global->Set(String::New("read"), FunctionTemplate::New(read));
	global->Set(String::New("write"), FunctionTemplate::New(write));
	global->Set(String::New("load"), FunctionTemplate::New(load));
}

}
