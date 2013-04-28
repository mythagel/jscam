/*
 * jsmachine.h
 *
 *  Created on: 28/04/2013
 *      Author: nicholas
 */

#ifndef JSMACHINE_H_
#define JSMACHINE_H_
#include <v8.h>

namespace jscam
{

void bind(v8::Handle<v8::ObjectTemplate> global);

}

#endif /* JSMACHINE_H_ */
