// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include "json_string.hpp"

#include <iostream>

using namespace metashell_system_test;

json_string::json_string(const std::string& json_) :
  _json(json_)
{}

const std::string& json_string::get() const
{
  return _json;
}

bool metashell_system_test::operator==(
  const json_string& a_,
  const json_string& b_
)
{
  return a_.get() == b_.get();
}

std::ostream& metashell_system_test::operator<<(
  std::ostream& out_,
  const json_string& s_
)
{
  return out_ << s_.get();
}

