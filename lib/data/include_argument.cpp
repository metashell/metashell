// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/include_argument.hpp>

#include <iostream>

using namespace metashell::data;

include_argument::include_argument(include_type type_,
                                   const boost::filesystem::path& path_)
  : type(type_), path(path_)
{
}

std::ostream& metashell::data::operator<<(std::ostream& out_,
                                          const include_argument& arg_)
{
  return out_ << include_code(arg_.type, arg_.path);
}

bool metashell::data::operator==(const include_argument& a_,
                                 const include_argument& b_)
{
  return a_.type == b_.type && a_.path == b_.path;
}
