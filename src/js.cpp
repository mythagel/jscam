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

v8::Local<v8::Value> v8_argument_iterator_adapter::iterator::operator*()
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

v8_argument_iterator_adapter arguments(const v8::Arguments& args)
{
	return { args };
}

v8::Handle<v8::Value> print(const v8::Arguments& args)
{
	using namespace v8;
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
