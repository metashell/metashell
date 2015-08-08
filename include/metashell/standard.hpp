#ifndef METASHELL_STANDARD_HPP
#define METASHELL_STANDARD_HPP

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

#include <iosfwd>
#include <string>

namespace metashell
{
  enum class standard {
    cpp11,
    cpp14
  };

  standard parse_standard(const std::string& std_);
  std::string clang_argument(standard std_);

  std::ostream& operator<<(std::ostream& os, standard std_);
}

#endif

