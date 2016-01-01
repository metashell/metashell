#ifndef METASHELL_SYSTEM_TEST_CALL_GRAPH_NODE_HPP
#define METASHELL_SYSTEM_TEST_CALL_GRAPH_NODE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/frame.hpp>

namespace metashell_system_test
{
  class call_graph_node
  {
  public:
    call_graph_node(const frame& frame_, int depth_, int number_of_children_);

    const frame& current_frame() const;
    int depth() const;
    int number_of_children() const;

  private:
    frame _frame;
    int _depth;
    int _number_of_children;
  };
}

#endif
