#ifndef METASHELL_TOKEN_CATEGORY_HPP
#define METASHELL_TOKEN_CATEGORY_HPP

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

#include <metashell/data/token_type.hpp>

namespace metashell
{
  namespace data
  {
    enum class token_category
    {
      character_literal,
      floating_literal,
      identifier,
      integer_literal,
      string_literal,
      bool_literal,
      unknown,
      whitespace,
      comment,
      keyword,
      operator_token,
      preprocessor
    };

    constexpr token_category category_of_token(token_type type_)
    {
      switch (type_)
      {
#ifdef METASHELL_TOKEN_TYPE
#error METASHELL_TOKEN_TYPE already defined
#endif
#define METASHELL_TOKEN_TYPE(name, cat, format)                                \
  case token_type::name:                                                       \
    return token_category::cat;

#include <metashell/data/impl/token_type_list.hpp>

#undef METASHELL_TOKEN_TYPE
      }
      // Avoid warning about not returning a value...
      return token_category::unknown;
    }

    bool whitespace_or_comment(token_category);
  }
}

#endif
