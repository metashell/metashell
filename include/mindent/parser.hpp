#ifndef MINDENT_PARSER_HPP
#define MINDENT_PARSER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file parser.hpp
 * @brief Interface for parsing angle-bracket expressions.
 */

#include <mindent/impl/parser.hpp>
#include <mindent/syntax_node_list.hpp>

#include <map>
#include <cassert>

namespace mindent
{
  /**
   * @brief Build a syntax tree from a Boost.Wave token sequence
   *
   * This function takes only the beginning of a token sequnce and assumes that
   * the entire sequence should be parsed.
   *
   * @tparam Lexer The lexer implementation to use.
   * @param lexer_ the lexer to use.
   */
  template <class Lexer>
  syntax_node_list<typename Lexer::token_type>
  parse_syntax_node_list(Lexer& lexer_)
  {
    const syntax_node_list<typename Lexer::token_type>
      p = impl::parse_syntax_node_list(lexer_);

    assert(!lexer_.has_further_tokens());

    return p;
  }
}

#endif

