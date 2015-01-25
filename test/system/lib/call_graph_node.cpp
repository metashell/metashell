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

#include <metashell_system_test/call_graph_node.hpp>

using namespace metashell_system_test;

call_graph_node::call_graph_node(
  const frame& frame_,
  int depth_,
  int number_of_children_
) :
  _frame(frame_),
  _depth(depth_),
  _number_of_children(number_of_children_)
{}

const frame& call_graph_node::current_frame() const
{
  return _frame;
}

int call_graph_node::depth() const
{
  return _depth;
}

int call_graph_node::number_of_children() const
{
  return _number_of_children;
}

