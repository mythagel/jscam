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
 * platform.cpp
 *
 *  Created on: 28/05/2013
 *      Author: nicholas
 */

#include "platform.h"
#include <cstdio>
#include <memory>
#include <libgen.h>
#include <vector>

// TODO platform specific stuff here.
// Investigate boost libs.
namespace platform
{

std::string realpath(const std::string& rel_path)
{
	auto cfree = [](char* c) { free(c); };
	std::unique_ptr<char, decltype(cfree)> path(::realpath(rel_path.c_str(), nullptr), cfree);
	
	return path.get();
}

std::string dirname(const std::string& file)
{
	std::vector<char> buf(std::begin(file), std::end(file));
	return ::dirname(buf.data());
}

}

