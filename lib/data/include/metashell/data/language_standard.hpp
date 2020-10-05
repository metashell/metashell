#ifndef METASHELL_DATA_LANGUAGE_STANDARD_HPP
#define METASHELL_DATA_LANGUAGE_STANDARD_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/real_engine_name.hpp>

#include <string>

namespace metashell
{
  namespace data
  {
    enum class language_standard
    {
      // C
      c89,
      gnu89,

      c90,
      iso9899_199409, // ISO C90 as modified in amendment 1 (g++)
      gnu90,

      c9x,
      gnu9x,

      c99,
      gnu99,

      c1x,
      gnu1x,

      c11,
      gnu11,

      // C++
      cpp98,
      gnu98,

      cpp03,
      gnu03,

      cpp0x,
      gnupp0x,

      cpp11,
      gnupp11,

      cpp1y,
      gnupp1y,

      cpp14,
      gnupp14,

      cpp1z,
      gnupp1z,

      cpp17,
      gnupp17,

      cpp2a,
      gnupp2a,

      cpp20,
      gnupp20
    };

    bool c_standard(language_standard);

    language_standard parse_standard(real_engine_name, const std::string&);

    command_line_argument gcc_name(language_standard);
    command_line_argument wave_name(language_standard);
  } // namespace data
} // namespace metashell

#endif
