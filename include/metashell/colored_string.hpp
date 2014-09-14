#ifndef METASHELL_COLORED_STRING_HPP
#define METASHELL_COLORED_STRING_HPP

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

#include <string>
#include <vector>
#include <ostream>

#include <boost/optional.hpp>

#include <just/console.hpp>

namespace metashell {

class colored_string {
public:
  typedef boost::optional<just::console::color> color_t;
  typedef std::vector<color_t> colors_t;

  typedef std::string::size_type size_type;

  colored_string() = default;
  colored_string(const std::string& string, const color_t& color = boost::none);

  colored_string& operator+=(const std::string& rhs);
  colored_string& operator+=(const colored_string& rhs);

  const std::string& get_string() const;
  const colors_t& get_colors() const;

  void print_to_cout() const;
  void print_to_cout(size_type begin, size_type length) const;

private:
  std::string string;
  colors_t colors;
};

std::ostream& operator<<(std::ostream& os, const colored_string& cs);

}

#endif
