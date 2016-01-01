// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/breakpoint.hpp>

#include <sstream>

namespace metashell
{

  breakpoint::breakpoint(int id, const boost::regex& name_regex)
    : id(id), name_regex(name_regex)
  {
  }

  bool breakpoint::match(const data::type& type) const
  {
    return name_regex && boost::regex_search(type.name(), *name_regex);
  }

  bool breakpoint::match(const data::frame& frame) const
  {
    return name_regex && boost::regex_search(frame.type().name(), *name_regex);
  }

  int breakpoint::get_id() const { return id; }

  std::string breakpoint::to_string() const
  {
    std::stringstream ss;
    ss << "Breakpoint " << id << ": ";
    if (name_regex)
    {
      ss << "regex(\"" + name_regex->str() + "\")";
    }
    else
    {
      ss << "unknown()";
    }
    return ss.str();
  }

} // namespace metashell
