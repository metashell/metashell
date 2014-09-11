#ifndef METASHELL_TEST_UTIL_HPP
#define METASHELL_TEST_UTIL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <boost/range/algorithm/equal.hpp>

std::string get_output(
  const std::string& input_,
  const std::string& test_code_ = std::string()
);

template<class C>
bool equal(const C& c1, std::initializer_list<typename C::value_type> c2) {
  return boost::equal(c1, c2);
}

#endif

