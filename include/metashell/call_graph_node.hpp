#ifndef METASHELL_CALL_GRAPH_NODE_HPP
#define METASHELL_CALL_GRAPH_NODE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/frame.hpp>

#include <boost/operators.hpp>

#include <iosfwd>

namespace metashell
{
  class call_graph_node : boost::equality_comparable<call_graph_node>
  {
  public:
    call_graph_node() = default;
    call_graph_node(
      const data::frame& frame_,
      int depth_,
      int number_of_children_
    );

    const data::frame& current_frame() const;
    int depth() const;
    int number_of_children() const;
  private:
    data::frame _frame;
    int _depth;
    int _number_of_children;
  };

  bool operator==(const call_graph_node& a_, const call_graph_node& b_);
  std::ostream& operator<<(std::ostream& o_, const call_graph_node& n_);
}

#endif

