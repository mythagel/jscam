#include <cstdio>
#include <v8.h>
#include "Machine.h"

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
int main()
{
	Machine machine{Machine::Type::Mill, "LinuxCNC"};

	using namespace v8;

	HandleScope handle_scope;

	Persistent<Context> context = Context::New();
	Context::Scope context_scope(context);

	Handle<String> source = String::New("'Hello' + ', World!'");
	Handle<Script> script = Script::Compile(source);
	Handle<Value> result = script->Run();

	context.Dispose();

	String::AsciiValue ascii(result);
	printf("%s\n", *ascii);
	return 0;
}
