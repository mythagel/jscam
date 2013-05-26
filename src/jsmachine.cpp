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
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	try
	{
		machine->AccuracyExactPath();
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
	return {};
}
Handle<Value> exact_stop(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	try
	{
		machine->AccuracyExactStop();
	}
	catch(const std::exception& ex)
	{
		return ThrowException(String::New(ex.what()));
	}
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

Handle<Value> motion(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	switch(machine->GetMotion())
	{
		case Machine::Motion::Absolute:
			return "absolute"_sym;
		case Machine::Motion::Incremental:
			return "incremental"_sym;
	}
	
	throw std::logic_error("Unknown motion type.");
}
void motion(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	if(value == "absolute"_sym)
	{
		machine->SetMotion(Machine::Motion::Absolute);
		return;
	}
	else if(value == "incremental"_sym)
	{
		machine->SetMotion(Machine::Motion::Incremental);
		return;
	}

	ThrowException(String::New("expected motion = absolute / incremental"));
}

Handle<Value> arc_motion(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	switch(machine->GetArcMotion())
	{
		case Machine::Motion::Absolute:
			return "absolute"_sym;
		case Machine::Motion::Incremental:
			return "incremental"_sym;
	}
	
	throw std::logic_error("Unknown arc motion type.");
}
void arc_motion(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	if(value == "absolute"_sym)
	{
		machine->SetArcMotion(Machine::Motion::Absolute);
		return;
	}
	else if(value == "incremental"_sym)
	{
		machine->SetArcMotion(Machine::Motion::Incremental);
		return;
	}

	ThrowException(String::New("expected arc_motion = absolute / incremental"));
}

Handle<Value> units(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	switch(machine->GetUnits())
	{
		case Machine::Units::Metric:
			return "metric"_sym;
		case Machine::Units::Imperial:
			return "imperial"_sym;
	}
	
	throw std::logic_error("Unknown units.");
}
void units(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	if(value == "metric"_sym)
	{
		machine->SetUnits(Machine::Units::Metric);
		return;
	}
	else if(value == "imperial"_sym)
	{
		machine->SetUnits(Machine::Units::Imperial);
		return;
	}

	ThrowException(String::New("expected units = metric / imperial"));
}

Handle<Value> plane(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	switch(machine->GetPlane())
	{
		case Machine::Plane::XY:
			return "XY"_sym;
		case Machine::Plane::ZX:
			return "ZX"_sym;
		case Machine::Plane::YZ:
			return "YZ"_sym;
		case Machine::Plane::UV:
			return "UV"_sym;
		case Machine::Plane::WU:
			return "WU"_sym;
		case Machine::Plane::VW:
			return "VW"_sym;
	}
	
	throw std::logic_error("Unknown plane.");
}
void plane(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	if(value == "XY"_sym)
	{
		machine->SetPlane(Machine::Plane::XY);
		return;
	}
	else if(value == "ZX"_sym)
	{
		machine->SetPlane(Machine::Plane::ZX);
		return;
	}
	else if(value == "YZ"_sym)
	{
		machine->SetPlane(Machine::Plane::YZ);
		return;
	}
	else if(value == "UV"_sym)
	{
		machine->SetPlane(Machine::Plane::UV);
		return;
	}
	else if(value == "WU"_sym)
	{
		machine->SetPlane(Machine::Plane::WU);
		return;
	}
	else if(value == "VW"_sym)
	{
		machine->SetPlane(Machine::Plane::VW);
		return;
	}

	ThrowException(String::New("expected plane = XY / ZX / YZ / UV / WU / VW"));
}

Handle<Value> feed_rate_mode(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	switch(machine->GetFeedRate().second)
	{
		case Machine::FeedRateMode::InverseTime:
			return "inverse"_sym;
		case Machine::FeedRateMode::UnitsPerMinute:
			return "upm"_sym;
		case Machine::FeedRateMode::UnitsPerRevolution:
			return "upr"_sym;
	}
	
	throw std::logic_error("Unknown feed rate mode.");
}
void feed_rate_mode(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	if(value == "inverse"_sym)
	{
		machine->SetFeedRateMode(Machine::FeedRateMode::InverseTime);
		return;
	}
	else if(value == "upm"_sym)
	{
		machine->SetFeedRateMode(Machine::FeedRateMode::UnitsPerMinute);
		return;
	}
	else if(value == "upr"_sym)
	{
		machine->SetFeedRateMode(Machine::FeedRateMode::UnitsPerRevolution);
		return;
	}

	ThrowException(String::New("expected feed_rate_mode = inverse / upm / upr"));
}

Handle<Value> feed_rate(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	return Number::New(machine->GetFeedRate().first);
}
void feed_rate(Local<String>, Local<Value> value, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);

	auto f = js::to_double(value);
	machine->SetFeedRate(f);
}

Handle<Value> spindle(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	auto s = machine->GetSpindleState();
	auto state = Object::New();
	state->Set("speed"_sym, Number::New(s.first));
	switch(s.second)
	{
		case Machine::Rotation::Stop:
			state->Set("direction"_sym, "stop"_sym);
			break;
		case Machine::Rotation::Clockwise:
			state->Set("direction"_sym, "clockwise"_sym);
			break;
		case Machine::Rotation::CounterClockwise:
			state->Set("direction"_sym, "counterclockwise"_sym);
			break;
	}
	
	return state;
}

Handle<Value> tool(Local<String>, const AccessorInfo& info)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(info);
	
	auto tl = machine->GetTool();
	auto tool = Object::New();
	tool->Set("name"_sym, String::New(tl.Name().c_str(), tl.Name().size()));
	switch(tl.ToolType())
	{
		case Tool::Type::Mill:
		{
			auto spec = tl.GetMill();
			tool->Set("type"_sym, "mill"_sym);
			
			tool->Set("center_cutting"_sym, Number::New(spec.center_cutting));
			tool->Set("flutes"_sym, Number::New(spec.flutes));
			tool->Set("flute_length"_sym, Number::New(spec.flute_length));
			tool->Set("cutting_length"_sym, Number::New(spec.cutting_length));
			tool->Set("mill_diameter"_sym, Number::New(spec.mill_diameter));
			tool->Set("shank_diameter"_sym, Number::New(spec.shank_diameter));
			tool->Set("core_diameter"_sym, Number::New(spec.core_diameter));
			tool->Set("length"_sym, Number::New(spec.length));
			break;
		}
		case Tool::Type::Lathe:
		{
			auto spec = tl.GetLathe();
			tool->Set("type"_sym, "lathe"_sym);
			// TODO fill to js
			break;
		}
	}
	
	return tool;
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
		auto r = args[1];
		if(r == "stop"_sym)
		{
			machine->StartSpindle(s, Machine::Rotation::Stop);
			return {};
		}
		else if(r == "clockwise"_sym)
		{
			machine->StartSpindle(s, Machine::Rotation::Clockwise);
			return {};
		}
		else if(r == "counterclockwise"_sym)
		{
			machine->StartSpindle(s, Machine::Rotation::CounterClockwise);
			return {};
		}
	}

	return ThrowException(String::New("expected spindle_on(uint s, r = machine.Rotation.Clockwise)"));
}
Handle<Value> spindle_off(const Arguments& args)
{
	HandleScope handle_scope;
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
			auto res = arg;
			if(res == "preserve"_sym)
			{
				machine->EndBlock(Machine::block_PreserveState);
				return {};
			}
			else if(res == "restore"_sym)
			{
				machine->EndBlock(Machine::block_RestoreState);
				return {};
			}
			else if(res == "units"_sym)
				restore |= Machine::block_RestoreUnits;
			else if(res == "plane"_sym)
				restore |= Machine::block_RestorePlane;
			else if(res == "motion"_sym)
				restore |= Machine::block_RestoreMotion;
			else if(res == "arc_motion"_sym)
				restore |= Machine::block_RestoreArcMotion;
			else if(res == "feedrate_mode"_sym)
				restore |= Machine::block_RestoreFeedRateMode;
			else if(res == "feedrate"_sym)
				restore |= Machine::block_RestoreFeedRate;
			else if(res == "spindle"_sym)
				restore |= Machine::block_RestoreSpindle;
			else if(res == "tool"_sym)
				restore |= Machine::block_RestoreTool;
			else if(res == "position"_sym)
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
		for(auto axis : js::array(keys))
		{
			auto value = axis_value->Get(axis);

			if(axis == "x"_sym)
				axes.push_back(X(js::to_double(value)));
			else if(axis == "y"_sym)
				axes.push_back(Y(js::to_double(value)));
			else if(axis == "z"_sym)
				axes.push_back(Z(js::to_double(value)));

			else if(axis == "a"_sym)
				axes.push_back(A(js::to_double(value)));
			else if(axis == "b"_sym)
				axes.push_back(B(js::to_double(value)));
			else if(axis == "c"_sym)
				axes.push_back(C(js::to_double(value)));

			// UVW unimplemented in cxxcam
//			else if(axis == "u"_sym)
//				axes.push_back(U(js::to_double(value)));
//			else if(axis == "v"_sym)
//				axes.push_back(V(js::to_double(value)));
//			else if(axis == "w"_sym)
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

		for(auto axis : js::array(keys))
		{
			auto value = axis_value->Get(axis);
			if(axis == "x"_sym)
				axes.push_back(X(js::to_double(value)));
			else if(axis == "y"_sym)
				axes.push_back(Y(js::to_double(value)));
			else if(axis == "z"_sym)
				axes.push_back(Z(js::to_double(value)));

			else if(axis == "a"_sym)
				axes.push_back(A(js::to_double(value)));
			else if(axis == "b"_sym)
				axes.push_back(B(js::to_double(value)));
			else if(axis == "c"_sym)
				axes.push_back(C(js::to_double(value)));

			// UVW unimplemented in cxxcam
//			else if(axis == "u"_sym)
//				axes.push_back(U(js::to_double(value)));
//			else if(axis == "v"_sym)
//				axes.push_back(V(js::to_double(value)));
//			else if(axis == "w"_sym)
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
	HandleScope handle_scope;
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

Handle<Value> generate_model(const Arguments& args)
{
	HandleScope handle_scope;
	auto machine = js::unwrap<Machine>(args);
	
	const auto stock = machine->GetStock();
	const auto stock_object = to_object(stock.Model);
	
	auto obj = Object::New();
	
	auto vertices = Array::New(stock_object.vertices.size());
	for(std::size_t v = 0; v != stock_object.vertices.size(); ++v)
	{
		auto& vtx = stock_object.vertices[v];
		auto vertex = Array::New(3);
		
		vertex->Set(0, Number::New(vtx.x));
		vertex->Set(1, Number::New(vtx.y));
		vertex->Set(2, Number::New(vtx.z));
		
		vertices->Set(v, vertex);
	}
	obj->Set("vertices"_sym, vertices);
	
	auto faces = Array::New(stock_object.faces.size());
	for(std::size_t f = 0; f != stock_object.faces.size(); ++f)
	{
		auto& fce = stock_object.faces[f];
		auto face = Array::New(fce.size());
		
		for(std::size_t f = 0; f != stock_object.faces.size(); ++f)
			face->Set(f, Number::New(fce[f]));
		
		faces->Set(f, face);
	}
	obj->Set("faces"_sym, faces);
	
	return handle_scope.Close(obj);
}

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
			auto type = tool_obj->Get("type"_sym);
			auto id = js::to_int32(tool_obj->Get("id"_sym));
		
			if(type == "mill"_sym)
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
			else if(type == "lathe"_sym)
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
	auto instance_template = tpl->InstanceTemplate();
	auto prototype = tpl->PrototypeTemplate();
	
	instance_template->SetInternalFieldCount(1);
	
	prototype->Set("exact_path"_sym, FunctionTemplate::New(exact_path)->GetFunction());
	prototype->Set("exact_stop"_sym, FunctionTemplate::New(exact_stop)->GetFunction());
	prototype->Set("path_blend"_sym, FunctionTemplate::New(path_blend)->GetFunction());
	
	instance_template->SetAccessor("motion"_sym, motion, motion);
	instance_template->SetAccessor("arc_motion"_sym, arc_motion, arc_motion);
	instance_template->SetAccessor("units"_sym, units, units);
	instance_template->SetAccessor("plane"_sym, plane, plane);
	instance_template->SetAccessor("feed_rate_mode"_sym, feed_rate_mode, feed_rate_mode);
	instance_template->SetAccessor("feed_rate"_sym, feed_rate, feed_rate);
	instance_template->SetAccessor("spindle"_sym, spindle);
	instance_template->SetAccessor("tool"_sym, tool);
	
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
