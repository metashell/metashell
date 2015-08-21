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

#include <metashell/pager.hpp>

#include <cassert>

#include <boost/range/algorithm/count.hpp>

#include <iostream>

namespace metashell {

pager::pager(iface::console& console) : console_(console) {}

void pager::show(const data::colored_string& string) {
  int width = console_.width();

  for (char ch : string.get_string()) {
    if (ch == '\n') {
      ++lines_in_current_page;
      chars_in_current_line = 0;
    } else {
      ++chars_in_current_line;
      if (chars_in_current_line > width) {
        ++lines_in_current_page;
        chars_in_current_line = 0;
      }
    }
  }

  console_.show(string);
}

void pager::new_line() {
  chars_in_current_line = 0;
  ++lines_in_current_page;

  console_.new_line();

  int height = console_.height();
  if (height <= lines_in_current_page + 1) {
    lines_in_current_page = 0;
  }
}

} // namespace metashell
