#ifndef METASHELL_RESULT_HPP
#define METASHELL_RESULT_HPP

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
#include <vector>

namespace metashell
{
  struct result
  {
    bool successful = false;

    std::string output;
    std::string error;
    std::string info;

    result();

    result(
      bool successful,
      const std::string& output_,
      const std::string& error_,
      const std::string& info_);
  };
}

#endif

