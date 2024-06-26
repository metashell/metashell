#ifndef METASHELL_DATA_CONSTRAINT_CODE_HPP
#define METASHELL_DATA_CONSTRAINT_CODE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/constraint/ascii_printable.hpp>
#include <metashell/data/constraint/extended_ascii.hpp>
#include <metashell/data/constraint/one_of.hpp>
#include <metashell/data/constraint/whitespace.hpp>

namespace metashell
{
  namespace data
  {
    namespace constraint
    {
      using code = one_of<whitespace, ascii_printable, extended_ascii>;
    }
  } // namespace data
} // namespace metashell

#endif
