#ifndef METASHELL_FORWARD_TRACE_ITERATOR_HPP
#define METASHELL_FORWARD_TRACE_ITERATOR_HPP

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

#include <metashell/data/call_graph_node.hpp>
#include <metashell/data/metaprogram.hpp>

#include <boost/operators.hpp>
#include <boost/optional.hpp>

#include <stack>
#include <tuple>

namespace metashell
{
  // -----
  // Customized DFS
  //   The algorithm only checks vertices which are reachable from root_vertex
  // ----
  class forward_trace_iterator
      : public boost::forward_iterator_helper<forward_trace_iterator,
                                              const data::call_graph_node>
  {
  public:
    forward_trace_iterator();

    forward_trace_iterator(const data::metaprogram& mp_,
                           const boost::optional<int>& max_depth_);

    forward_trace_iterator& operator++();

    bool operator==(const forward_trace_iterator& i_) const;

    const data::call_graph_node& operator*() const;

  private:
    typedef std::tuple<data::metaprogram::optional_edge_descriptor,
                       int // Depth
                       >
        stack_element;

    data::call_graph_node _current;
    bool _finished;

    boost::optional<int> _max_depth;

    // The usual stack for DFS
    std::stack<stack_element> _to_visit;
    const data::metaprogram* _mp = nullptr;
    data::metaprogram::discovered_t _discovered;

    void visit(const data::metaprogram::optional_edge_descriptor& edge_,
               int depth_);
  };
}

#endif
