#include <v8.h>
#include "js.h"
#include "jsmachine.h"
#include <iostream>
#include <iterator>

v8::Persistent<v8::Context> create_context();
v8::Handle<v8::String> read_stdin();

int main(int argc, char* argv[])
{
	using namespace v8;
	V8::SetFlagsFromCommandLine(&argc, argv, true);

	bool result = false;
	{
		HandleScope handle_scope;
		auto context = create_context();
		Context::Scope context_scope(context);

		auto source = read_stdin();
		result = js::exec(source, String::New("stdin"));

		context.Dispose();
	}
	V8::Dispose();
	return result ? 0 : 1;
}

v8::Persistent<v8::Context> create_context()
{
	using namespace v8;
	auto global = ObjectTemplate::New();

	js::bind(global);
	jscam::bind(global);

	return Context::New(NULL, global);
}

v8::Handle<v8::String> read_stdin()
{
	auto str = std::string(std::istreambuf_iterator<char>(std::cin.rdbuf()), std::istreambuf_iterator<char>());
	return v8::String::New(str.c_str(), str.size());
}
