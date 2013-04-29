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

namespace
{

Handle<String> read_file(const char* name)
{
	std::ifstream ifs(name);
	if(!ifs)
		return {};

	auto str = std::string(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());
	return String::New(str.c_str(), str.size());
}

}

namespace js
{

std::string to_string(v8::Local<v8::Value> s)
{
	if(!s->IsString())
		return {};
	auto str = s->ToString();
	String::AsciiValue ascii(str);
	return {*ascii, *ascii + ascii.length()};
}

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

v8_argument_iterator_adapter arguments(const Arguments& args)
{
	return { args };
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

	auto source = read_file(*file);
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

		auto source = read_file(*file);
		if (source.IsEmpty())
			return ThrowException(String::New("Unable to read file"));

		if (!exec(source, String::New(*file)))
			return ThrowException(String::New("Unable to execute file"));
	}
	return handle_scope.Close(Undefined());
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
}

}
