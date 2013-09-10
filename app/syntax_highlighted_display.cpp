// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "syntax_highlighted_display.hpp"

console::color syntax_highlighted_display::color_of_token(
  boost::wave::token_id id_
)
{
  if (IS_CATEGORY(id_, boost::wave::CharacterLiteralTokenType))
  {
    return console::magenta;
  }
  else if (IS_CATEGORY(id_, boost::wave::FloatingLiteralTokenType))
  {
    return console::magenta;
  }
  else if (IS_CATEGORY(id_, boost::wave::IntegerLiteralTokenType))
  {
    return console::magenta;
  }
  else if (IS_CATEGORY(id_, boost::wave::StringLiteralTokenType))
  {
    return console::magenta;
  }
  else if (IS_CATEGORY(id_, boost::wave::BoolLiteralTokenType))
  {
    return console::magenta;
  }
  else if (IS_CATEGORY(id_, boost::wave::IdentifierTokenType))
  {
    return console::white;
  }
  else if (IS_CATEGORY(id_, boost::wave::KeywordTokenType))
  {
    return console::bright_green;
  }
  else if (IS_CATEGORY(id_, boost::wave::OperatorTokenType))
  {
    return console::white;
  }
  else if (IS_CATEGORY(id_, boost::wave::PPTokenType))
  {
    return console::magenta;
  }
  else
  {
    return console::white;
  }
}


