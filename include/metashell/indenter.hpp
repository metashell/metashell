#ifndef METASHELL_INDENTER_HPP
#define METASHELL_INDENTER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/shell.hpp>
#include <metashell/token_iterator.hpp>

#include <mindent/display.hpp>
#include <mindent/parser.hpp>
#include <mindent/syntax_node.hpp>
#include <mindent/syntax_node_list.hpp>

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
    return
      mindent::display(
        mindent::parse_syntax_node_list(begin_tokens(s_)),
        width_,
        indent_step_,
        f_
      );
  }
}

#endif

