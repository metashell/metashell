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

#include "mock_cxtranslationunit.hpp"

using namespace metashell;

mock_cxtranslationunit::mock_cxtranslationunit() {}

void mock_cxtranslationunit::visit_nodes(const visitor&)
{
  // ignore
}

std::string mock_cxtranslationunit::get_error_string() const {
  return _error;
}

void mock_cxtranslationunit::code_complete(std::set<std::string>&) const
{
  // ignore
}

void mock_cxtranslationunit::set_error(const std::string& msg_)
{
  _error = msg_;
}

