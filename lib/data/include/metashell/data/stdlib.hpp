#ifndef METASHELL_DATA_STDLIB_HPP
#define METASHELL_DATA_STDLIB_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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
#include <metashell/data/string.hpp>

namespace metashell
{
  namespace data
  {
    class stdlib : string<stdlib,
                          true,
                          constraint::any,
                          constraint::any,
                          command_line_argument>
    {
    public:
      using string<stdlib,
                   true,
                   constraint::any,
                   constraint::any,
                   command_line_argument>::string;
      using string<stdlib,
                   true,
                   constraint::any,
                   constraint::any,
                   command_line_argument>::value;

      static constexpr const char* name_of_type() { return "-stdlib value"; }
    };
  } // namespace data
} // namespace metashell

#endif
