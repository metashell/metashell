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

#include <boost/range/algorithm/equal.hpp>

#include <algorithm>

namespace metashell {

colored_string::colored_string(
    const std::string& string, const color_t& color) :
  string(string), colors(string.size(), color) {}

colored_string::colored_string(
    const char *string, const color_t& color) :
  colored_string(std::string(string), color) {}

colored_string& colored_string::operator+=(const char *rhs) {
  string += rhs;
  colors.resize(string.size(), boost::none);
  return *this;
}

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

colored_string::size_type colored_string::size() const {
  assert(string.size() == colors.size());
  return string.size();
}

const std::string& colored_string::get_string() const {
  return string;
}

const colored_string::colors_t& colored_string::get_colors() const {
  return colors;
}

void print_to_cout(const colored_string& s_) {
  colored_string::color_t prev_color = boost::none;
  auto color_it = s_.get_colors().begin();

  for (
    auto char_it = s_.get_string().begin(), e = s_.get_string().end();
    char_it != e;
    ++char_it, ++color_it
  ) {
    if (*color_it != prev_color) {
      if (prev_color) {
        just::console::reset();
      }
      if (*color_it) {
        just::console::text_color(**color_it);
      }
      prev_color = *color_it;
    }
    std::cout << *char_it;
  }
  if (prev_color) {
    just::console::reset();
  }
}

std::ostream& operator<<(std::ostream& os, const colored_string& cs) {
  return os << cs.get_string();
}

colored_string colored_string::substr(size_type pos_, size_type len_) const
{
  const auto b = std::min(pos_, size());
  const auto e = std::min(b + len_, size());

  return
    colored_string(
      string.begin() + b,
      string.begin() + e,
      colors.begin() + b,
      colors.begin() + e
    );
}

void colored_string::clear()
{
  string.clear();
  colors.clear();
}

bool operator==(const colored_string& a_, const colored_string& b_)
{
  return
    boost::equal(a_.get_string(), b_.get_string())
    && boost::equal(a_.get_colors(), b_.get_colors());
}

}
