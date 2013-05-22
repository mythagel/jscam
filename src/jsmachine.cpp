/*
 * jsmachine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#include "jsmachine.h"
#include <memory>
#include "Machine.h"
#include "Axis.h"
#include "Offset.h"
#include "Tool.h"
#include "js.h"

using namespace v8;

namespace jscam
{

Handle<Value> exact_path(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);
	machine->AccuracyExactPath();
	return {};
}
Handle<Value> exact_stop(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);
	machine->AccuracyExactStop();
	return {};
}
Handle<Value> path_blend(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 0)
	{
		machine->AccuracyPathBlending();
		return {};
	}
	else if(args.Length() == 1)
	{
		auto p = js::to_double(args[0]);
		machine->AccuracyPathBlending(p);
		return {};
	}
	else if(args.Length() == 2)
	{
		auto p = js::to_double(args[0]);
		auto q = js::to_double(args[1]);
		machine->AccuracyPathBlending(p, q);
		return {};
	}

	return ThrowException(String::New("expected path_blend(void / double p / double p, double q)"));
}

Handle<Value> motion(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);

	auto motion = js::to_string(args[0]);
	if(motion == "absolute")
	{
		machine->SetMotion(Machine::Motion::Absolute);
		return {};
	}
	else if(motion == "incremental")
	{
		machine->SetMotion(Machine::Motion::Incremental);
		return {};
	}

	return ThrowException(String::New("expected motion(absolute / incremental)"));
}
Handle<Value> arc_motion(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);

	auto motion = js::to_string(args[0]);
	if(motion == "absolute")
	{
		machine->SetArcMotion(Machine::Motion::Absolute);
		return {};
	}
	else if(motion == "incremental")
	{
		machine->SetArcMotion(Machine::Motion::Incremental);
		return {};
	}

	return ThrowException(String::New("expected arc_motion(absolute / incremental)"));
}
Handle<Value> units(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);

	auto units = js::to_string(args[0]);
	if(units == "metric")
	{
		machine->SetUnits(Machine::Units::Metric);
		return {};
	}
	else if(units == "imperial")
	{
		machine->SetUnits(Machine::Units::Imperial);
		return {};
	}

	return ThrowException(String::New("expected units(metric / imperial)"));
}
Handle<Value> plane(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);

	auto plane = js::to_string(args[0]);
	if(plane == "XY")
	{
		machine->SetPlane(Machine::Plane::XY);
		return {};
	}
	else if(plane == "ZX")
	{
		machine->SetPlane(Machine::Plane::ZX);
		return {};
	}
	else if(plane == "YZ")
	{
		machine->SetPlane(Machine::Plane::YZ);
		return {};
	}
	else if(plane == "UV")
	{
		machine->SetPlane(Machine::Plane::UV);
		return {};
	}
	else if(plane == "WU")
	{
		machine->SetPlane(Machine::Plane::WU);
		return {};
	}
	else if(plane == "VW")
	{
		machine->SetPlane(Machine::Plane::VW);
		return {};
	}

	return ThrowException(String::New("expected plane(XY / ZX / YZ / UV / WU / VW)"));
}
Handle<Value> feed_rate_mode(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);

	auto mode = js::to_string(args[0]);
	if(mode == "inverse")
	{
		machine->SetFeedRateMode(Machine::FeedRateMode::InverseTime);
		return {};
	}
	else if(mode == "upm")
	{
		machine->SetFeedRateMode(Machine::FeedRateMode::UnitsPerMinute);
		return {};
	}
	else if(mode == "upr")
	{
		machine->SetFeedRateMode(Machine::FeedRateMode::UnitsPerRevolution);
		return {};
	}

	return ThrowException(String::New("expected feed_rate_mode(inverse / upm / upr)"));
}
Handle<Value> feed_rate(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 1)
	{
		auto f = js::to_double(args[0]);
		machine->SetFeedRate(f);
		return {};
	}

	return ThrowException(String::New("expected feed_rate(double feedrate)"));
}

Handle<Value> spindle_on(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 1)
	{
		auto s = js::to_uint32(args[0]);
		machine->StartSpindle(s);
		return {};
	}
	else if(args.Length() == 2)
	{
		auto s = js::to_uint32(args[0]);
		auto r = js::to_string(args[1]);
		if(r == "stop")
		{
			machine->StartSpindle(s, Machine::Rotation::Stop);
			return {};
		}
		else if(r == "clockwise")
		{
			machine->StartSpindle(s, Machine::Rotation::Clockwise);
			return {};
		}
		else if(r == "counterclockwise")
		{
			machine->StartSpindle(s, Machine::Rotation::CounterClockwise);
			return {};
		}
	}

	return ThrowException(String::New("expected spindle_on(uint s, r = machine.Rotation.Clockwise)"));
}
Handle<Value> spindle_off(const Arguments& args)
{
	auto machine = js::unwrap<Machine>(args);
	machine->StopSpindle();
	return {};
}

Handle<Value> load_tool(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 1)
	{
		auto id = js::to_uint32(args[0]);
		machine->SetTool(id);
		return {};
	}

	return ThrowException(String::New("expected load_tool(int id)"));
}
Handle<Value> tool_change(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 1)
	{
		auto id = js::to_uint32(args[0]);
		machine->ToolChange(id);
		return {};
	}

	return ThrowException(String::New("expected tool_change(int id)"));
}

Handle<Value> begin_block(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 1)
	{
		auto name = js::to_string(args[0]);
		machine->NewBlock(name);
		return {};
	}

	return ThrowException(String::New("expected begin_block(string name)"));
}
Handle<Value> end_block(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 0)
	{
		machine->EndBlock();
		return {};
	}
	else
	{
		int restore(0);
		for(auto arg : js::arguments(args))
		{
			auto res = js::to_string(arg);
			if(res == "preserve")
			{
				machine->EndBlock(Machine::block_PreserveState);
				return {};
			}
			else if(res == "restore")
			{
				machine->EndBlock(Machine::block_RestoreState);
				return {};
			}
			else if(res == "units")
				restore |= Machine::block_RestoreUnits;
			else if(res == "plane")
				restore |= Machine::block_RestorePlane;
			else if(res == "motion")
				restore |= Machine::block_RestoreMotion;
			else if(res == "arc_motion")
				restore |= Machine::block_RestoreArcMotion;
			else if(res == "feedrate_mode")
				restore |= Machine::block_RestoreFeedRateMode;
			else if(res == "feedrate")
				restore |= Machine::block_RestoreFeedRate;
			else if(res == "spindle")
				restore |= Machine::block_RestoreSpindle;
			else if(res == "tool")
				restore |= Machine::block_RestoreTool;
			else if(res == "position")
				restore |= Machine::block_RestorePosition;
		}
		machine->EndBlock(restore);
		return {};
	}

	return ThrowException(String::New("expected end_block(void / restore)"));
}

Handle<Value> optional_pause(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	if(args.Length() == 0)
	{
		machine->OptionalPause();
		return {};
	}
	else if(args.Length() == 1)
	{
		auto comment = js::to_string(args[0]);
		machine->OptionalPause(comment);
		return {};
	}

	return ThrowException(String::New("expected optional_pause(string comment)"));
}

Handle<Value> rapid(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	std::vector<Axis> axes;
	for(auto arg : js::arguments(args))
	{
		auto axis_value = arg->ToObject();
		auto keys = axis_value->GetPropertyNames();
		for(auto key : js::array(keys))
		{
			auto axis = js::to_string(key);
			auto value = axis_value->Get(key);

			if(axis == "x")
				axes.push_back(X(js::to_double(value)));
			else if(axis == "y")
				axes.push_back(Y(js::to_double(value)));
			else if(axis == "z")
				axes.push_back(Z(js::to_double(value)));

			else if(axis == "a")
				axes.push_back(A(js::to_double(value)));
			else if(axis == "b")
				axes.push_back(B(js::to_double(value)));
			else if(axis == "c")
				axes.push_back(C(js::to_double(value)));

			// UVW unimplemented in cxxcam
//			else if(axis == "u")
//				axes.push_back(U(js::to_double(value)));
//			else if(axis == "v")
//				axes.push_back(V(js::to_double(value)));
//			else if(axis == "w")
//				axes.push_back(W(js::to_double(value)));

			else
				return ThrowException(String::New("Unrecognised axis."));
		}
	}
	machine->Rapid(axes);

	return {};
}
Handle<Value> linear(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	std::vector<Axis> axes;
	for(auto arg : js::arguments(args))
	{
		auto axis_value = arg->ToObject();
		auto keys = axis_value->GetPropertyNames();

		for(auto key : js::array(keys))
		{
			auto axis = js::to_string(key);
			auto value = axis_value->Get(key);
			if(axis == "x")
				axes.push_back(X(js::to_double(value)));
			else if(axis == "y")
				axes.push_back(Y(js::to_double(value)));
			else if(axis == "z")
				axes.push_back(Z(js::to_double(value)));

			else if(axis == "a")
				axes.push_back(A(js::to_double(value)));
			else if(axis == "b")
				axes.push_back(B(js::to_double(value)));
			else if(axis == "c")
				axes.push_back(C(js::to_double(value)));

			// UVW unimplemented in cxxcam
//			else if(axis == "u")
//				axes.push_back(U(js::to_double(value)));
//			else if(axis == "v")
//				axes.push_back(V(js::to_double(value)));
//			else if(axis == "w")
//				axes.push_back(W(js::to_double(value)));

			else
				return ThrowException(String::New("Unrecognised axis."));
		}
	}
	machine->Linear(axes);

	return {};
}
Handle<Value> arc(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);

	Machine::Direction dir = Machine::Direction::Clockwise;
	std::vector<Axis> end_pos;
	std::vector<Offset> center;
	unsigned int turns = 1;

	/*
	 * TODO this is messy and horrible.
	 * Define a common binding strategy and stick to it.
	 */
	unsigned int index(0);
	for(auto arg : js::arguments(args))
	{
		++index;

		if(index == 1)
		{
			auto d = js::to_string(arg);
			if(d == "clockwise")
			{
				dir = Machine::Direction::Clockwise;
			}
			else if(d == "counterclockwise")
			{
				dir = Machine::Direction::CounterClockwise;
			}
			else
			{
				return ThrowException(String::New("Unrecognised direction"));
			}
		}
		else if(arg->IsObject())
		{
			auto axis_value = arg->ToObject();
			auto keys = axis_value->GetPropertyNames();

			for(auto key : js::array(keys))
			{
				auto axis = js::to_string(key);
				auto value = axis_value->Get(key);

				if(axis == "x")
					end_pos.push_back(X(js::to_double(value)));
				else if(axis == "y")
					end_pos.push_back(Y(js::to_double(value)));
				else if(axis == "z")
					end_pos.push_back(Z(js::to_double(value)));

				else if(axis == "a")
					end_pos.push_back(A(js::to_double(value)));
				else if(axis == "b")
					end_pos.push_back(B(js::to_double(value)));
				else if(axis == "c")
					end_pos.push_back(C(js::to_double(value)));

				// UVW unimplemented in cxxcam
	//			else if(axis == "u")
	//				end_pos.push_back(U(js::to_double(value)));
	//			else if(axis == "v")
	//				end_pos.push_back(V(js::to_double(value)));
	//			else if(axis == "w")
	//				end_pos.push_back(W(js::to_double(value)));

				// Offsets
				else if(axis == "i")
					center.push_back(I(js::to_double(value)));
				else if(axis == "j")
					center.push_back(J(js::to_double(value)));
				else if(axis == "k")
					center.push_back(K(js::to_double(value)));

				else if(axis == "turns")
					turns = js::to_uint32(value);

				else
					return ThrowException(String::New("Unrecognised axis / offset / turns."));
			}
		}
		else
		{

		}
	}
	machine->Arc(dir, end_pos, center, turns);

	return {};
}
Handle<Value> plunge(const Arguments&)
{
	//auto machine = js::unwrap<Machine>(args);

	// TODO missing in cxxcam
	return {};
}
Handle<Value> generate(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	using gcode::Word;
	
	auto word2js = [](const Word& word) -> Handle<Object>
	{
		auto js_word = Object::New();
		
		auto c = to_string(word);
		js_word->Set(String::NewSymbol(c.c_str(), c.size()), Number::New(word.Value()));
		
		if(!word.Comment().empty())
			js_word->Set(String::NewSymbol("comment"), String::New(word.Comment().c_str(), word.Comment().size()));
		
		return js_word;
	};
	auto line2js = [word2js](const Machine::line_t& line) -> Handle<Array>
	{
		if(line.empty())
			return Array::New();
		
		auto js_line = Array::New(line.words.size() + (line.comment.empty() ? 0 : 1));

		for(std::size_t w = 0; w != line.words.size(); ++w)
			js_line->Set(w, word2js(line.words[w]));
		
		if(!line.comment.empty())
			js_line->Set(line.words.size(), String::New(line.comment.c_str(), line.comment.size()));
		
		return js_line;
	};
	
	auto lines = machine->Generate();
	
	auto js_lines = Array::New(lines.size());
	for(std::size_t l = 0; l != lines.size(); ++l)
		js_lines->Set(l, line2js(lines[l]));

	return handle_scope.Close(js_lines);
}

//Handle<Value> GetPointX(Local<String> property, const AccessorInfo &info)
//{
//	Local<Object> self = info.Holder();
//	Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
//	void* ptr = wrap->Value();
//	int value = static_cast<Point*>(ptr)->x_;
//	return Integer::New(value);
//}

void bind(Handle<Object> global)
{
	// Name the class in js
	auto name = String::NewSymbol("Machine");
	
	auto tpl = FunctionTemplate::New([](const Arguments& args) -> Handle<Value>
	{
		if (!args.IsConstructCall())
			return ThrowException(String::New("Cannot call constructor as function"));

		HandleScope scope;

		auto config = args[0]->ToObject();
		if(config.IsEmpty())
			return ThrowException(String::New("Expected config, variant."));
		auto variant = js::to_string(args[1]);

		auto type = js::to_string(config->Get(String::NewSymbol("type")));

		if(type == "mill")
			js::make_object<Machine>(args.This(), Machine::Type::Mill, variant);
		else if(type == "lathe")
			js::make_object<Machine>(args.This(), Machine::Type::Lathe, variant);
		else
			return ThrowException(String::New("type - mill / lathe"));

		auto machine = js::unwrap<Machine>(args);

		auto tools = config->Get(String::NewSymbol("tools"));
		for(auto t : js::array(tools))
		{
			Tool tool;
			auto tool_obj = t->ToObject();

			auto name = js::to_string(tool_obj->Get(String::NewSymbol("name")));
			auto type = js::to_string(tool_obj->Get(String::NewSymbol("type")));
			auto id = js::to_int32(tool_obj->Get(String::NewSymbol("id")));
		
			if(type == "mill")
			{
				auto spec = Tool::Mill();

				spec.type = Tool::Mill::Type::End;
				spec.center_cutting = js::to_bool(tool_obj->Get(String::NewSymbol("center_cutting")));
				spec.flutes = js::to_uint32(tool_obj->Get(String::NewSymbol("flutes")));
				spec.flute_length = js::to_double(tool_obj->Get(String::NewSymbol("flute_length")));
				spec.cutting_length = js::to_double(tool_obj->Get(String::NewSymbol("cutting_length")));
				spec.mill_diameter = js::to_double(tool_obj->Get(String::NewSymbol("mill_diameter")));
				spec.shank_diameter = js::to_double(tool_obj->Get(String::NewSymbol("shank_diameter")));
				spec.core_diameter = js::to_double(tool_obj->Get(String::NewSymbol("core_diameter")));
				spec.length = js::to_double(tool_obj->Get(String::NewSymbol("length")));
			
				tool = Tool(name, spec);
			}
			else if(type == "lathe")
			{
				// TODO fill spec from js
				auto spec = Tool::Lathe();
				tool = Tool(name, spec);
			}

			machine->AddTool(id, tool);
		}

		auto spindle_speeds = config->Get(String::NewSymbol("spindle"));
		for(auto s : js::array(spindle_speeds))
		{
			if(s->IsNumber())
			{
				auto speed = js::to_uint32(s);
				machine->AddSpindleDiscrete(speed);
			}
			else
			{
				auto speed = js::to_string(s);
				auto dash_pos = speed.find('-');
				if(dash_pos != std::string::npos)
				{
					auto start = speed.substr(0, dash_pos);
					auto end = speed.substr(dash_pos+1);
					machine->AddSpindleRange(std::stoul(start), std::stoul(end));
				}
				else
				{
					return ThrowException(String::New("range: 'start-end'"));
				}
			}
		}

		return args.This();
	});
	
	tpl->SetClassName(name);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
	auto prototype = tpl->PrototypeTemplate();
	
	prototype->Set(String::NewSymbol("exact_path"), FunctionTemplate::New(exact_path)->GetFunction());
	prototype->Set(String::NewSymbol("exact_stop"), FunctionTemplate::New(exact_stop)->GetFunction());
	prototype->Set(String::NewSymbol("path_blend"), FunctionTemplate::New(path_blend)->GetFunction());
	prototype->Set(String::NewSymbol("motion"), FunctionTemplate::New(motion)->GetFunction());
	prototype->Set(String::NewSymbol("arc_motion"), FunctionTemplate::New(arc_motion)->GetFunction());
	prototype->Set(String::NewSymbol("units"), FunctionTemplate::New(units)->GetFunction());
	prototype->Set(String::NewSymbol("plane"), FunctionTemplate::New(plane)->GetFunction());
	prototype->Set(String::NewSymbol("feed_rate_mode"), FunctionTemplate::New(feed_rate_mode)->GetFunction());
	prototype->Set(String::NewSymbol("feed_rate"), FunctionTemplate::New(feed_rate)->GetFunction());
	prototype->Set(String::NewSymbol("spindle_on"), FunctionTemplate::New(spindle_on)->GetFunction());
	prototype->Set(String::NewSymbol("spindle_off"), FunctionTemplate::New(spindle_off)->GetFunction());
	prototype->Set(String::NewSymbol("load_tool"), FunctionTemplate::New(load_tool)->GetFunction());
	prototype->Set(String::NewSymbol("tool_change"), FunctionTemplate::New(tool_change)->GetFunction());
	prototype->Set(String::NewSymbol("begin_block"), FunctionTemplate::New(begin_block)->GetFunction());
	prototype->Set(String::NewSymbol("end_block"), FunctionTemplate::New(end_block)->GetFunction());
	prototype->Set(String::NewSymbol("optional_pause"), FunctionTemplate::New(optional_pause)->GetFunction());
	prototype->Set(String::NewSymbol("rapid"), FunctionTemplate::New(rapid)->GetFunction());
	prototype->Set(String::NewSymbol("linear"), FunctionTemplate::New(linear)->GetFunction());
	prototype->Set(String::NewSymbol("arc"), FunctionTemplate::New(arc)->GetFunction());
	prototype->Set(String::NewSymbol("plunge"), FunctionTemplate::New(plunge)->GetFunction());
	prototype->Set(String::NewSymbol("generate"), FunctionTemplate::New(generate)->GetFunction());
	
	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
}

}
