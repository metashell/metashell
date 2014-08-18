#ifndef METASHELL_INDENTER_HPP
#define METASHELL_INDENTER_HPP

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

#include <metashell/shell.hpp>
#include <metashell/command.hpp>

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
    const std::string& s_,
    const std::string& input_filename_
  )
  {
    const command cmd(s_);

    return
      mindent::display(
        mindent::parse_syntax_node_list(cmd.begin()),
        width_,
        indent_step_,
        f_
      );
  }
}

#endif

