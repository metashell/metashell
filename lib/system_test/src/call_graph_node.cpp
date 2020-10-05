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

#include <metashell/system_test/call_graph_node.hpp>

namespace metashell
{
  namespace system_test
  {
    call_graph_node::call_graph_node(const frame& frame_,
                                     int depth_,
                                     int number_of_children_)
      : _frame(frame_), _depth(depth_), _number_of_children(number_of_children_)
    {
    }

    const frame& call_graph_node::current_frame() const { return _frame; }

    int call_graph_node::depth() const { return _depth; }

    int call_graph_node::number_of_children() const
    {
      return _number_of_children;
    }

    bool operator==(const call_graph_node& a_, const call_graph_node& b_)
    {
      return a_.depth() == b_.depth() &&
             a_.number_of_children() == b_.number_of_children() &&
             a_.current_frame() == b_.current_frame();
    }
  } // namespace system_test
} // namespace metashell
