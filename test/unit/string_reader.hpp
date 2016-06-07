#ifndef METASHELL_TEST_STRING_READER_HPP
#define METASHELL_TEST_STRING_READER_HPP

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

#include <boost/optional.hpp>

#include <string>
#include <vector>

class string_reader
{
public:
  explicit string_reader(std::initializer_list<std::string> strings_);

  boost::optional<std::string> operator()(const std::string&);

private:
  std::vector<std::string> _strings;
  // storing the index instead of an iterator makes the default copy
  // constructor work
  std::vector<std::string>::size_type _next;
};

#endif
