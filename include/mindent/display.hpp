#ifndef MINDENT_DISPLAY_HPP
#define MINDENT_DISPLAY_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file display.hpp
 * @brief Interface for indenting syntax trees for a fixed width display.
 */

#include <mindent/impl/display.hpp>
#include <mindent/syntax_node_list.hpp>

namespace mindent
{
  /**
   * @brief Function for indenting a syntax tree for a fixed width display.
   *
   * This function indents the syntax tree using a fixed maximum width. Things
   * that can be displayed in one line are kept in one line. Otherwise they are
   * split into multiple lines at token boundaries where possible.
   *
   * When a template instance is displayed in multiple lines each template
   * argument goes into a different line.
   *
   * This function uses spaces for indentation.
   *
   * This function determines the indentation and splits tokens into multiple
   * ones when they are longer than the maximum width. This function does not
   * deal with displaying the tokens. It calls a functor for each token and
   * expects that functor to display them.
   *
   * The library provides pre-defined funtors: stream_display and
   * string_display.
   *
   * @tparam DisplayF the type of the functor that displays one token.
   * @tparam TokenType the type used for representeing tokens.
   *
   * @param l_ the syntax tree to be displayed.
   * @param width_ the maximum width of the displayed code.
   * @param indent_step_ the number of spaces to use for indentation.
   * @param show_ the functor displaying the tokens.
   */
  template <class DisplayF, class TokenType>
  DisplayF display(
    const syntax_node_list<TokenType>& l_,
    int width_,
    int indent_step_,
    DisplayF show_
  )
  {
    return impl::display(l_, width_, indent_step_, 0, width_, show_);
  }
}

#endif

