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

bool exec(v8::Handle<v8::String> source, v8::Handle<v8::Value> name);
void bind(v8::Handle<v8::ObjectTemplate> global);

}

#endif /* JS_H_ */
