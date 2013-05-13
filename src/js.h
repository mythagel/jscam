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

v8::Handle<v8::String> read_stream(std::istream& is);

std::string to_string(v8::Local<v8::Value> s);
double to_double(v8::Local<v8::Value> d);
int32_t to_int32(v8::Local<v8::Value> i);
uint32_t to_uint32(v8::Local<v8::Value> i);
bool to_bool(v8::Local<v8::Value> b);

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

}

#endif /* JS_H_ */
