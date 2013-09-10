#ifndef METASHELL_INDENTER_HPP
#define METASHELL_INDENTER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/shell.hpp>

#include <mindent/display.hpp>
#include <mindent/parser.hpp>
#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

namespace metashell
{
  template <class DisplayF>
  DisplayF indent(
    int width_,
    int indent_step_,
    DisplayF f_,
    const std::string& s_
  )
  {
    typedef boost::wave::cpplexer::lex_token<> token_type;
    typedef boost::wave::cpplexer::lex_iterator<token_type> iterator_type;

    return
      mindent::display(
        mindent::parse_syntax_node_list(
          iterator_type(
            s_.begin(),
            s_.end(),
            token_type::position_type(shell::input_filename()),
            boost::wave::language_support(
              boost::wave::support_cpp
              | boost::wave::support_option_long_long
            )
          )
        ),
        width_,
        indent_step_,
        f_
      );
  }
}

#endif

