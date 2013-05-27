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
 * js.h
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#ifndef JS_H_
#define JS_H_
#include <v8.h>
#include <string>
#include <cstdint>
#include <iosfwd>

namespace js
{

// TODO better name
inline v8::Handle<v8::String> operator"" _sym(const char* s, size_t l)
{
	return v8::String::NewSymbol(s, l);
}

v8::Handle<v8::String> read_stream(std::istream& is);

std::string to_string(v8::Handle<v8::Value> s);
double to_double(v8::Handle<v8::Value> d);
int32_t to_int32(v8::Handle<v8::Value> i);
uint32_t to_uint32(v8::Handle<v8::Value> i);
bool to_bool(v8::Handle<v8::Value> b);

namespace detail
{

struct v8_argument_iterator_adapter
{
	const v8::Arguments& args;

	struct iterator
	{
		const v8_argument_iterator_adapter& a;
		int index;

		v8::Local<v8::Value> operator*();
		iterator& operator++();
		bool operator!=(const iterator& o) const;
	};
};

v8_argument_iterator_adapter::iterator begin(const v8_argument_iterator_adapter& a);
v8_argument_iterator_adapter::iterator end(const v8_argument_iterator_adapter& a);

struct v8_object_iterator_adapter
{
	v8::Local<v8::Array> obj;

	struct iterator
	{
		const v8_object_iterator_adapter& a;
		uint32_t index;

		v8::Local<v8::Value> operator*();
		iterator& operator++();
		bool operator!=(const iterator& o) const;
	};
};

v8_object_iterator_adapter::iterator begin(const v8_object_iterator_adapter& a);
v8_object_iterator_adapter::iterator end(const v8_object_iterator_adapter& a);

}

detail::v8_argument_iterator_adapter arguments(const v8::Arguments& args);
detail::v8_object_iterator_adapter array(v8::Local<v8::Value> obj);

bool exec(v8::Handle<v8::String> source, v8::Handle<v8::Value> name);
void bind(v8::Handle<v8::ObjectTemplate> global);

/*
 * Retrieve the c++ object pointer from the js object
 */
template <typename T>
static T* unwrap(const v8::Arguments& args)
{
	auto self = args.Holder();
	auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	return static_cast<T*>(wrap->Value());
}
template <typename T>
static T* unwrap(const v8::AccessorInfo& info)
{
	auto self = info.Holder();
	auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	return static_cast<T*>(wrap->Value());
}

/*
 * Construct a new c++ object and wrap it in a js object
 */
template <typename T, typename... Args>
static v8::Persistent<v8::Object> make_object(v8::Handle<v8::Object> object, Args&&... args)
{
	auto x = new T(std::forward<Args>(args)...);
	auto obj = v8::Persistent<v8::Object>::New(object);
	obj->SetInternalField(0, v8::External::New(x));

	obj.MakeWeak(x, [](v8::Persistent<v8::Value> obj, void* data)
	{
		auto x = static_cast<T*>(data);
		delete x;

		obj.Dispose();
		obj.Clear();
	});

	return obj;
}

}

#endif /* JS_H_ */
