/*
 * js.h
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#ifndef JS_H_
#define JS_H_
#include <v8.h>

namespace js
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

v8_argument_iterator_adapter arguments(const v8::Arguments& args);

bool exec(v8::Handle<v8::String> source, v8::Handle<v8::Value> name);
void bind(v8::Handle<v8::ObjectTemplate> global);

}

#endif /* JS_H_ */
