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

#include <metashell/console_type.hpp>

#include <iostream>
#include <stdexcept>

using namespace metashell;

std::ostream& metashell::operator<<(std::ostream& o_, console_type t_)
{
  switch (t_)
  {
  case console_type::plain: return o_ << "plain";
  case console_type::readline: return o_ << "readline";
  }
  return o_; // avoid "control reaches end of function" warnings
}

console_type metashell::parse_console_type(const std::string& con_type_)
{
  if (con_type_ == "plain")
  {
    return console_type::plain;
  }
  else if (con_type_ == "readline")
  {
    return console_type::readline;
  }
  else
  {
    throw std::runtime_error("Not supported console type " + con_type_);
  }
}

