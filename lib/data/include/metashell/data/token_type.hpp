#ifndef METASHELL_TOKEN_TYPE_HPP
#define METASHELL_TOKEN_TYPE_HPP

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

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    enum class token_type
    {
#ifdef METASHELL_TOKEN_TYPE
#error METASHELL_TOKEN_TYPE already defined
#endif
#define METASHELL_TOKEN_TYPE(name, cat, format) name TOKEN_TYPE_SEP

#include <metashell/data/impl/token_type_list.hpp>

#undef METASHELL_TOKEN_TYPE
    };

    std::string to_string(token_type t_);
    std::ostream& operator<<(std::ostream&, token_type);
  }
}

#endif
