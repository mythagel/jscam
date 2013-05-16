#include <v8.h>
#include <vector>
#include "js.h"
#include <fstream>
#include <iostream>
#include "jsmachine.h"

v8::Persistent<v8::Context> create_context();

int main(int argc, char* argv[])
{
	using namespace v8;
	V8::SetFlagsFromCommandLine(&argc, argv, true);

	std::vector<std::string> args(argv, argv+argc);
	args.erase(args.begin());

	bool result = false;
	if(args.empty())
	{
		HandleScope handle_scope;
		auto context = create_context();
		Context::Scope context_scope(context);

		auto source = js::read_stream(std::cin);
		result = js::exec(source, String::NewSymbol("stdin"));

		context.Dispose();
	}
	else
	{
		HandleScope handle_scope;
		auto context = create_context();
		Context::Scope context_scope(context);

		for(auto& arg : args)
		{
			std::ifstream ifs(arg);
			auto source = js::read_stream(ifs);
			if(source.IsEmpty())
			{
				std::cerr << "Unable to read source file " << arg << '\n';
				result = false;
				break;
			}

			result = js::exec(source, String::New(arg.c_str(), arg.size()));
			if(!result)
			{
				std::cerr << "Unable to execute source file " << arg << '\n';
				break;
			}
		}

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
