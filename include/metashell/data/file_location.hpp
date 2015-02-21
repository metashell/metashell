
#ifndef METASHELL_FILE_LOCATION_HPP
#define METASHELL_FILE_LOCATION_HPP

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
#include <ostream>

#include <boost/operators.hpp>

namespace metashell {
namespace data {

struct file_location : boost::totally_ordered<file_location> {
  file_location();
  file_location(const std::string& name, int row, int column);

  std::string name;
  int row;
  int column;

};

bool operator<(const file_location& lhs, const file_location& rhs);
bool operator==(const file_location& lhs, const file_location& rhs);
std::ostream& operator<<(std::ostream& os, const file_location& location);

std::string to_string(const file_location& location);

}
}

#endif
