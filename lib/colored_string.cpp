// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/colored_string.hpp>

namespace metashell {

colored_string::colored_string(
    const std::string& string, const color_t& color) :
  string(string), colors(string.size(), color) {}

colored_string& colored_string::operator+=(const std::string& rhs) {
  string += rhs;
  colors.resize(string.size(), boost::none);
  return *this;
}

colored_string& colored_string::operator+=(const colored_string& rhs) {
  string += rhs.string;
  colors.insert(colors.end(), rhs.colors.begin(), rhs.colors.end());
  return *this;
}

const std::string& colored_string::get_string() const {
  return string;
}

const colored_string::colors_t& colored_string::get_colors() const {
  return colors;
}

void colored_string::print_to_cout() const {
  print_to_cout(0, string.size());
}

void colored_string::print_to_cout(size_type begin, size_type length) const {
  assert(string.size() == colors.size());

  size_type full_length = string.size();
  color_t prev_color = boost::none;
  for (size_type i = 0; i < length && begin + i < full_length; ++i) {
    const color_t& color = colors[begin + i];
    if (color != prev_color) {
      if (prev_color) {
        just::console::reset();
      }
      if (color) {
        just::console::text_color(*color);
      }
      prev_color = color;
    }
    std::cout << string[i];
  }
  if (prev_color) {
    just::console::reset();
  }
}

std::ostream& operator<<(std::ostream& os, const colored_string& cs) {
  if (&os == &std::cout) {
    cs.print_to_cout();
  } else {
    os << cs.get_string();
  }
  return os;
}

}
