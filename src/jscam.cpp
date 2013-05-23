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

#include <v8.h>
#include <vector>
#include "js.h"
#include <fstream>
#include <iostream>
#include "jsmachine.h"

using namespace v8;

using js::operator"" _sym;

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
			result = js::exec(source, "stdin"_sym);
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
