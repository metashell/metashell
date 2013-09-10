#ifndef METASHELL_SYNTAX_HIGHLIGHTED_DISPLAY_HPP
#define METASHELL_SYNTAX_HIGHLIGHTED_DISPLAY_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"
#include "console.hpp"

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <iostream>

class syntax_highlighted_display
{
public:
  template <class TokenType>
  void operator()(const TokenType& t_)
  {
    console::text_color(color_of_token(t_));
    std::cout << t_.get_value();
  }
private:
  static console::color color_of_token(boost::wave::token_id id_);
};

#endif

