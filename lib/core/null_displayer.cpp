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

#include <metashell/null_displayer.hpp>

using namespace metashell;

void null_displayer::show_raw_text(const std::string&)
{
  // throw away
}

void null_displayer::show_error(const std::string&)
{
  // throw away
}

void null_displayer::show_type(const data::type&)
{
  // throw away
}

void null_displayer::show_comment(const data::text&)
{
  // throw away
}

void null_displayer::show_cpp_code(const std::string&)
{
  // throw away
}

void null_displayer::show_frame(const data::frame&)
{
  // throw away
}

void null_displayer::show_file_section(
  const data::file_location&, const std::string&)
{
  // throw away
}

void null_displayer::show_backtrace(const data::backtrace&)
{
  // throw away
}

void null_displayer::show_call_graph(const iface::call_graph&)
{
  // throw away
}

