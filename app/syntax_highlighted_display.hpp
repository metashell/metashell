#ifndef METASHELL_SYNTAX_HIGHLIGHTED_DISPLAY_HPP
#define METASHELL_SYNTAX_HIGHLIGHTED_DISPLAY_HPP

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

#include "readline_shell.hpp"

#include <just/console.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/optional.hpp>

#include <iostream>

class syntax_highlighted_display
{
public:
  template <class TokenType>
  void operator()(const TokenType& t_)
  {
    if (const boost::optional<just::console::color> c = color_of_token(t_))
    {
      just::console::text_color(*c);
    }
    else
    {
      just::console::reset();
    }
    std::cout << t_.get_value();
  }
private:
  static boost::optional<just::console::color> color_of_token(
    boost::wave::token_id id_
  );
};

#endif

