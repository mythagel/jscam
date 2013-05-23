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
#include "Stock.h"
#include "nef/io.h"
#include "js.h"
#include <stdexcept>

using namespace v8;

using js::operator"" _sym;

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
			js_word->Set("comment"_sym, String::New(word.Comment().c_str(), word.Comment().size()));
		
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

/*
 * Format: https://github.com/mrdoob/three.js/wiki/JSON-Model-format-3.1
 */
Handle<Value> generate_model(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	auto stock = machine->GetStock();
	auto stock_object = to_object(stock.Model);
	
	auto obj = Object::New();
	
	{
		auto vertices = Array::New(stock_object.vertices.size()*3);
		std::size_t idx{0};
		for(auto& vertex : stock_object.vertices)
		{
			vertices->Set(idx++, Number::New(vertex.x));
			vertices->Set(idx++, Number::New(vertex.y));
			vertices->Set(idx++, Number::New(vertex.z));
		}
		obj->Set("vertices"_sym, vertices);
	}
	
	std::vector<std::size_t> all_faces;
	for(auto& face : stock_object.faces)
	{
		if(face.size() == 3)
		{
			all_faces.push_back(0);
			all_faces.insert(all_faces.end(), begin(face), end(face));
		}
		else if(face.size() == 4)
		{
			all_faces.push_back(0);
			all_faces.insert(all_faces.end(), begin(face), end(face));
		}
		else
		{
			throw std::runtime_error("Unhandled face geometry.");
		}
	}
	
	{
		auto faces = Array::New(all_faces.size());
		for(std::size_t f = 0; f != all_faces.size(); ++f)
			faces->Set(f, Number::New(all_faces[f]));
		obj->Set("faces"_sym, faces);
	}
	
	return handle_scope.Close(obj);
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
	auto name = "Machine"_sym;
	
	auto tpl = FunctionTemplate::New([](const Arguments& args) -> Handle<Value>
	{
		if (!args.IsConstructCall())
			return ThrowException(String::New("Cannot call constructor as function"));

		HandleScope scope;

		auto config = args[0]->ToObject();
		if(config.IsEmpty())
			return ThrowException(String::New("Expected config, variant."));
		auto variant = js::to_string(args[1]);

		auto type = js::to_string(config->Get("type"_sym));

		if(type == "mill")
			js::make_object<Machine>(args.This(), Machine::Type::Mill, variant);
		else if(type == "lathe")
			js::make_object<Machine>(args.This(), Machine::Type::Lathe, variant);
		else
			return ThrowException(String::New("type - mill / lathe"));

		auto machine = js::unwrap<Machine>(args);

		auto tools = config->Get("tools"_sym);
		for(auto t : js::array(tools))
		{
			Tool tool;
			auto tool_obj = t->ToObject();

			auto name = js::to_string(tool_obj->Get("name"_sym));
			auto type = js::to_string(tool_obj->Get("type"_sym));
			auto id = js::to_int32(tool_obj->Get("id"_sym));
		
			if(type == "mill")
			{
				auto spec = Tool::Mill();

				spec.type = Tool::Mill::Type::End;
				spec.center_cutting = js::to_bool(tool_obj->Get("center_cutting"_sym));
				spec.flutes = js::to_uint32(tool_obj->Get("flutes"_sym));
				spec.flute_length = js::to_double(tool_obj->Get("flute_length"_sym));
				spec.cutting_length = js::to_double(tool_obj->Get("cutting_length"_sym));
				spec.mill_diameter = js::to_double(tool_obj->Get("mill_diameter"_sym));
				spec.shank_diameter = js::to_double(tool_obj->Get("shank_diameter"_sym));
				spec.core_diameter = js::to_double(tool_obj->Get("core_diameter"_sym));
				spec.length = js::to_double(tool_obj->Get("length"_sym));
			
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

		auto spindle_speeds = config->Get("spindle"_sym);
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
	
	prototype->Set("exact_path"_sym, FunctionTemplate::New(exact_path)->GetFunction());
	prototype->Set("exact_stop"_sym, FunctionTemplate::New(exact_stop)->GetFunction());
	prototype->Set("path_blend"_sym, FunctionTemplate::New(path_blend)->GetFunction());
	prototype->Set("motion"_sym, FunctionTemplate::New(motion)->GetFunction());
	prototype->Set("arc_motion"_sym, FunctionTemplate::New(arc_motion)->GetFunction());
	prototype->Set("units"_sym, FunctionTemplate::New(units)->GetFunction());
	prototype->Set("plane"_sym, FunctionTemplate::New(plane)->GetFunction());
	prototype->Set("feed_rate_mode"_sym, FunctionTemplate::New(feed_rate_mode)->GetFunction());
	prototype->Set("feed_rate"_sym, FunctionTemplate::New(feed_rate)->GetFunction());
	prototype->Set("spindle_on"_sym, FunctionTemplate::New(spindle_on)->GetFunction());
	prototype->Set("spindle_off"_sym, FunctionTemplate::New(spindle_off)->GetFunction());
	prototype->Set("load_tool"_sym, FunctionTemplate::New(load_tool)->GetFunction());
	prototype->Set("tool_change"_sym, FunctionTemplate::New(tool_change)->GetFunction());
	prototype->Set("begin_block"_sym, FunctionTemplate::New(begin_block)->GetFunction());
	prototype->Set("end_block"_sym, FunctionTemplate::New(end_block)->GetFunction());
	prototype->Set("optional_pause"_sym, FunctionTemplate::New(optional_pause)->GetFunction());
	prototype->Set("rapid"_sym, FunctionTemplate::New(rapid)->GetFunction());
	prototype->Set("linear"_sym, FunctionTemplate::New(linear)->GetFunction());
	prototype->Set("arc"_sym, FunctionTemplate::New(arc)->GetFunction());
	prototype->Set("plunge"_sym, FunctionTemplate::New(plunge)->GetFunction());
	prototype->Set("generate"_sym, FunctionTemplate::New(generate)->GetFunction());
	prototype->Set("generate_model"_sym, FunctionTemplate::New(generate_model)->GetFunction());
	
	auto constructor = Persistent<Function>::New(tpl->GetFunction());
	global->Set(name, constructor);
}

}
