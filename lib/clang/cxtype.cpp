// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/clang/cxtype.hpp>
#include <metashell/clang/cxcursor.hpp>
#include <metashell/clang/cxstring.hpp>

#include <sstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/transform_iterator.hpp>

using namespace metashell::clang;

cxtype::cxtype() {}

cxtype::cxtype(CXType type_) : _type(type_) {}

std::string cxtype::spelling() const
{
  return cxcursor(clang_getTypeDeclaration(_type)).spelling();
}

cxtype cxtype::canonical_type() const
{
  return cxtype(clang_getCanonicalType(_type));
}

