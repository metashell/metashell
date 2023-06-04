#ifndef METASHELL_DATA_MACRO_DEF_OR_UNDEF_HPP
#define METASHELL_DATA_MACRO_DEF_OR_UNDEF_HPP

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

#include <metashell/data/macro_definition.hpp>
#include <metashell/data/macro_undefinition.hpp>

#include <iosfwd>
#include <string>
#include <variant>

namespace metashell
{
  namespace data
  {
    using macro_def_or_undef =
        std::variant<macro_definition, macro_undefinition>;

    command_line_argument clang_argument(const macro_def_or_undef&);

    std::string to_string(const macro_def_or_undef&);
    std::ostream& operator<<(std::ostream&, const macro_def_or_undef&);
  } // namespace data
} // namespace metashell

#endif
