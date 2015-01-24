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

#include <metashell/null_cxtranslationunit.hpp>

using namespace metashell;

void null_cxtranslationunit::visit_nodes(const visitor&)
{
  // ignore
}

null_cxtranslationunit::error_iterator
null_cxtranslationunit::errors_begin() const
{
  return errors_end();
}

null_cxtranslationunit::error_iterator
null_cxtranslationunit::errors_end() const
{
  return error_iterator();
}

void null_cxtranslationunit::code_complete(std::set<std::string>&) const
{
  // ignore
}

