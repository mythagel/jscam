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
 * jspath.h
 *
 *  Created on: 2014-12-15
 *      Author: nicholas
 */

#ifndef JSPATH_H_
#define JSPATH_H_
#include <v8.h>

namespace jspath
{

/*
API Sketch

step = {
	position = {x:0,y:0,z:0};
	orientation = {a:0,b:0,c:0,d:0};
};

path_t = {
	path = [step];
	length = 0;
	angular_length = 0; // degrees
};

path_t expand_linear(start {xyzabc}, end {xyzabc}, geometry "XYZABC", steps_per_mm = 10);

path_t expand_rotary(start {xyzabc}, end {xyzabc}, geometry "XYZABC", steps_per_degree = 10);

path_t expand_arc(start {xyzabc}, end {xyzabc}, center {xyz}, clockwise = true, plane {xyz}, turns 1, geometry "XYZABC", steps_per_mm = 10);


*/
void bind(v8::Handle<v8::Object> global);

}

#endif /* JSPATH_H_ */
