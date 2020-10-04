#ifndef METASHELL_DATA_CONSTRAINT_NAME_FIRST_HPP
#define METASHELL_DATA_CONSTRAINT_NAME_FIRST_HPP

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

#include <metashell/data/constraint/exactly.hpp>
#include <metashell/data/constraint/letter.hpp>
#include <metashell/data/constraint/one_of.hpp>

namespace metashell
{
  namespace data
  {
    namespace constraint
    {
      using name_first = one_of<letter, exactly<'_'>>;
    }
  } // namespace data
} // namespace metashell

#endif
