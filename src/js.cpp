/*
 * js.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "js.h"
#include <iostream>
#include <fstream>
#include <iterator>

using namespace v8;

/*
 * Much of this code is based on v8 examples.
 */

namespace js
{

Handle<String> read_stream(std::istream& is)
{
	if(!is)
		return {};

	auto str = std::string(std::istreambuf_iterator<char>(is.rdbuf()), std::istreambuf_iterator<char>());
	return String::New(str.c_str(), str.size());
}

std::string to_string(Local<Value> s)
{
	if(!s->IsString())
		return {};
	auto str = s->ToString();
	String::AsciiValue ascii(str);
	return {*ascii, *ascii + ascii.length()};
}

double to_double(Local<Value> d)
{
	return d->NumberValue();
}

int32_t to_int32(Local<Value> i)
{
	return i->IntegerValue();
}

uint32_t to_uint32(v8::Local<v8::Value> i)
{
	return i->Uint32Value();
}

bool to_bool(v8::Local<v8::Value> b)
{
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
	if (args.Length() != 1)
		return ThrowException(String::New("Expected: filename"));

	String::Utf8Value file(args[0]);
	if (*file == nullptr)
		return ThrowException(String::New("Expected: string"));

	std::ifstream ifs(*file);
	auto source = read_stream(ifs);
	if (source.IsEmpty())
		return ThrowException(String::New("Unable to read file"));

	return source;
}
Handle<Value> load(const Arguments& args)
{
	HandleScope handle_scope;

	for(auto arg : arguments(args))
	{
		HandleScope handle_scope;
		String::Utf8Value file(arg);
		if (*file == nullptr)
			return ThrowException(String::New("Expected: string"));

		std::ifstream ifs(*file);
		auto source = read_stream(ifs);
		if (source.IsEmpty())
			return ThrowException(String::New("Unable to read file"));

		if (!exec(source, String::New(*file)))
			return ThrowException(String::New("Unable to execute file"));
	}
	return handle_scope.Close(Undefined());
}
Handle<Value> require(const Arguments& args)
{
	HandleScope handle_scope;

	String::Utf8Value file(args[0]);
	if (*file == nullptr)
		return ThrowException(String::New("Expected: string"));

	std::ifstream ifs(*file);
	auto source = read_stream(ifs);
	if (source.IsEmpty())
		return ThrowException(String::New("Unable to read file"));

	TryCatch try_catch;

	auto name = String::New(*file);
	auto script = Script::Compile(source, name);
	if (script.IsEmpty())
	{
		auto exception = try_catch.Exception();
		String::AsciiValue exception_str(exception);
		std::cerr << *exception_str << std::endl;
		
		return ThrowException(exception);
	}

	auto result = script->Run();
	if (result.IsEmpty())
	{
		auto exception = try_catch.Exception();
		String::AsciiValue exception_str(exception);
		std::cerr << *exception_str << std::endl;
		
		return ThrowException(exception);
	}

	return handle_scope.Close(result);
}

bool exec(Handle<String> source, Handle<Value> name)
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

	auto result = script->Run();
	if (result.IsEmpty())
	{
		auto exception = try_catch.Exception();
		String::AsciiValue exception_str(exception);
		std::cerr << *exception_str << std::endl;
		return false;
	}

	return true;
}

void bind(Handle<ObjectTemplate> global)
{
	global->Set(String::New("print"), FunctionTemplate::New(print));
	global->Set(String::New("read"), FunctionTemplate::New(read));
	global->Set(String::New("load"), FunctionTemplate::New(load));
	global->Set(String::New("require"), FunctionTemplate::New(require));
}

}
