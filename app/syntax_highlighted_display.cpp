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

#include "syntax_highlighted_display.hpp"

#include <just/console.hpp>

#include <boost/optional.hpp>

#include <iostream>

using namespace metashell;

namespace
{
  boost::optional<just::console::color> color_of_token(const token& t_)
  {
    using just::console::color;
    using boost::optional;
  
    switch (t_.category())
    {
    case token_category::character_literal:
    case token_category::floating_literal:
    case token_category::integer_literal:
    case token_category::string_literal:
    case token_category::bool_literal:
    case token_category::preprocessor:
      return color::magenta;
    case token_category::keyword:
      return color::bright_green;
    case token_category::comment:
      return color::green;
    default:
      return optional<color>();
    }
  }
}

void syntax_highlighted_display::operator()(const token& t_)
{
  if (const boost::optional<just::console::color> c = color_of_token(t_))
  {
    just::console::text_color(*c);
  }
  else
  {
    just::console::reset();
  }
  std::cout << t_.value();
}

