#include <v8.h>
#include <vector>
#include "js.h"
#include <fstream>
#include <iostream>
#include "jsmachine.h"

using namespace v8;

/*
 * Create a new js context with basic functions.
 */
Persistent<Context> create_context();

int main(int argc, char* argv[])
{
	V8::SetFlagsFromCommandLine(&argc, argv, true);

	std::vector<std::string> args(argv, argv+argc);
	args.erase(args.begin());

	bool result = false;
	{
		HandleScope handle_scope;
		auto context = create_context();
		Context::Scope context_scope(context);
		
		auto global = context->Global();
		jscam::bind(global);
	
		if(args.empty())
		{
			auto source = js::read_stream(std::cin);
			result = js::exec(source, String::NewSymbol("stdin"));
		}
		else
		{
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
		}
		context.Dispose();
	}

	while (!V8::IdleNotification())
		;

	V8::Dispose();
	return result ? 0 : 1;
}

Persistent<Context> create_context()
{
	auto global_template = ObjectTemplate::New();
	js::bind(global_template);
	return Context::New(NULL, global_template);
}
