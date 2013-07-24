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
 * jsgeom.h
 *
 *  Created on: 2013-07-24
 *      Author: nicholas
 */

#ifndef JSGEOM_H_
#define JSGEOM_H_
#include <v8.h>

namespace jsgeom
{

/*
API Sketch

Polyhedron
{
	Polyhedron intersection(const Polyhedron& poly) const;
	Polyhedron union(const Polyhedron& poly) const;
	Polyhedron difference(const Polyhedron& poly) const;
	Polyhedron symmetric_difference(const Polyhedron& poly) const;
	Polyhedron complement() const;

	bool empty() const;
	
	array<Polyhedron> explode();
	
	object_t to_object();
	void write_off(filename);
	
	Polyhedron glide([{x:1, y:1, z:1}, {x:10, y:10, z:10}]);
	
	double volume();
	
	Polyhedron rotate({w: 0, x:1, y:1, z:1});
	Polyhedron translate({x:1, y:1, z:1});
}

Polyhedron make_sphere({x:10, y:10, z:10}, double r, std::size_t slices);
Polyhedron make_box(const primitives::point_3& p1, const primitives::point_3& p2);
Polyhedron make_cone(const primitives::point_3& top_center, const primitives::point_3& bottom_center, double top_radius, double bottom_radius, std::size_t slices);

*/
void bind(v8::Handle<v8::Object> global);

}

#endif /* JSGEOM_H_ */
