// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/command.hpp>

#include <metashell/highlight_syntax.hpp>

namespace metashell {

colored_string::color_t color_of_token(const token& t) {
  using just::console::color;

  switch (t.category())
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
    return boost::none;
  }
}

colored_string highlight_syntax(const std::string& str) {
  colored_string result;

  const command cmd(str);
  for (const token& t : cmd) {
    result += colored_string(t.value(), color_of_token(t));
  }

  return result;
}

void display_syntax_highlighted(const token& t) {
  std::cout << colored_string(t.value(), color_of_token(t));
}

}

