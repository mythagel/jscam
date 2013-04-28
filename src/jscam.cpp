#include <v8.h>
#include "jsmachine.h"
#include "js.h"
#include "Machine.h"
#include <iostream>
#include <iterator>

v8::Persistent<v8::Context> create_context();
v8::Handle<v8::String> read_stdin();

/*
 js api

 "machine":
 {
 "type": "mill/lathe",
 "tools":
 [
 {
 "name": "tool_name",
 "type": "mill/lathe",

 "center_cutting": "true/false",
 "flutes": "int",
 "flute_length": "",
 "cutting_length": "",
 "mill_diameter": "",
 "shank_diameter": "",
 "core_diameter": "",
 "length": ""
 },
 {...}
 ],
 "spindle": "0-100,300,500-1000,3000"
 }

 setup(machine, "LinuxCNC")

 exact_path()
 exact_stop()
 path_blend()
 path_blend(p)
 path_blend(p, q)

 motion(absoloute/incremental)
 arc_motion(absoloute/incremental)

 units(metric/imperial)

 plane(XY/XZ...)

 feed_rate_mode(...)

 feed_rate(30.5);

 spindle_on(speed, direction)
 spindle_off()

 load_tool(id)
 tool_change(id)

 begin_block(name)
 end_block(restore)

 optional_pause(comment)

 rapid(xyzabc)
 rapid( {"x": "5"}, {"y": "50"} )

 linear(xyzabc)

 arc(...)

 plunge(...)



 */
int main(int argc, char* argv[])
{
	using namespace v8;
	V8::SetFlagsFromCommandLine(&argc, argv, true);

	bool result{};
	{
		std::cout << "jscam" << std::endl;

		// TODO investigate isolate versions.
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
