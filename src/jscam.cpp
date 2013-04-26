#include <cstdio>
#include <v8.h>
#include "Machine.h"

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
