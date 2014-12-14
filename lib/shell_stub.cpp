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

#include <metashell/shell_stub.hpp>
#include <metashell/null_displayer.hpp>

using namespace metashell;

namespace
{
  iface::displayer& get_null_displayer()
  {
    static null_displayer nd;
    return nd;
  }
}

shell_stub::shell_stub(const std::string& argv0_) :
  shell(empty_config(argv0_), get_null_displayer())
{}

void shell_stub::add_history(const std::string&)
{}

unsigned int shell_stub::width() const
{
  return 80;
}

