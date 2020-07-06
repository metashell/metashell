// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include "string_reader.hpp"

string_reader::string_reader(
    std::initializer_list<metashell::data::user_input> strings_)
  : _strings(strings_), _next(0)
{
}

std::optional<metashell::data::user_input> string_reader::
operator()(const std::string&)
{
  if (_next == _strings.size())
  {
    return std::nullopt;
  }
  else
  {
    return _strings[_next++];
  }
}
