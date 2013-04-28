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

/*
 * Much of this code is based on v8 examples.
 */

namespace
{

v8::Handle<v8::String> read_file(const char* name)
{
	std::ifstream ifs(name);
	if(!ifs)
		return {};

	auto str = std::string(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());
	return v8::String::New(str.c_str(), str.size());
}

}

namespace js
{

v8::Handle<v8::Value> print(const v8::Arguments& args)
{
	using namespace v8;

	bool first = true;
	for (int i = 0; i != args.Length(); ++i)
	{
		HandleScope handle_scope;
		if (first)
			first = false;
		else
			std::cout << ' ';

		String::Utf8Value str(args[i]);
		std::cout << *str;
	}
	std::cout << std::endl;

	return Undefined();
}
v8::Handle<v8::Value> read(const v8::Arguments& args)
{
	using namespace v8;

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
v8::Handle<v8::Value> load(const v8::Arguments& args)
{
	using namespace v8;

	for (int i = 0; i != args.Length(); ++i)
	{
		HandleScope handle_scope;
		String::Utf8Value file(args[i]);
		if (*file == nullptr)
			return ThrowException(String::New("Expected: string"));

		auto source = read_file(*file);
		if (source.IsEmpty())
			return ThrowException(String::New("Unable to read file"));

		if (!exec(source, String::New(*file)))
			return ThrowException(String::New("Unable to execute file"));
	}
	return Undefined();
}

bool exec(v8::Handle<v8::String> source, v8::Handle<v8::Value> name)
{
	using namespace v8;

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

void bind(v8::Handle<v8::ObjectTemplate> global)
{
	using namespace v8;

	global->Set(String::New("print"), FunctionTemplate::New(print));
	global->Set(String::New("read"), FunctionTemplate::New(read));
	global->Set(String::New("load"), FunctionTemplate::New(load));
}

}
