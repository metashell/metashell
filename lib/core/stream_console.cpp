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

#include <metashell/stream_console.hpp>

#include <ostream>
#include <limits>

using namespace metashell;

stream_console::stream_console(std::ostream& s_) :
  _s(&s_)
{}

void stream_console::show(const data::colored_string& s_)
{
  *_s << s_.get_string();
}

void stream_console::new_line()
{
  *_s << std::endl;
}

int stream_console::width() const
{
  return std::numeric_limits<int>::max();
}

int stream_console::height() const
{
  return std::numeric_limits<int>::max();
}
