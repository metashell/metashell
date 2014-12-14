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

#include <metashell/forward_trace_iterator.hpp>

#include <boost/range/adaptor/reversed.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  int number_of_children(
    const metashell::metaprogram& mp_,
    const metashell::metaprogram::graph_t& graph_,
    const metashell::metaprogram::vertex_descriptor& vertex_
  )
  {
    typedef metashell::metaprogram::edge_descriptor edsc;

    const auto oe = boost::out_edges(vertex_, graph_);
    return
      std::count_if(
        oe.first,
        oe.second,
        [&mp_](const edsc& ed) { return mp_.get_edge_property(ed).enabled; }
      );
  }
}

forward_trace_iterator::forward_trace_iterator() :
  _finished(true)
{}

forward_trace_iterator::forward_trace_iterator(
  const metaprogram& mp_,
  const boost::optional<int>& max_depth_
) :
  _finished(false),
  _max_depth(max_depth_),
  _graph(&mp_.get_graph()),
  _mp(&mp_),
  _discovered(mp_.get_state().discovered)
{
  visit(_mp->get_current_edge(), 0);
}

void forward_trace_iterator::visit(
  const metaprogram::optional_edge_descriptor& edge_,
  int depth_
)
{
  metaprogram::vertex_descriptor vertex = 
    edge_ ? _mp->get_target(*edge_) : _mp->get_root_vertex();

  _current =
    call_graph_node(
      edge_ ? _mp->to_frame(*edge_) : _mp->get_root_frame(),
      depth_,
      (_discovered[vertex] || (_max_depth && *_max_depth <= depth_)) ?
        0 : number_of_children(*_mp, *_graph, vertex)
    );

  if (!_discovered[vertex])
  {
    if (!_mp->is_in_full_mode())
    {
      _discovered[vertex] = true;
    }

    if (!_max_depth || *_max_depth > depth_)
    {
      metaprogram::out_edge_iterator begin, end;
      std::tie(begin, end) = boost::out_edges(vertex, *_graph);

      std::vector<metaprogram::edge_descriptor> edges(begin, end);

      // Reverse iteration, so types that got instantiated first
      // get on the top of the stack
      for (
        const metaprogram::edge_descriptor& out_edge :
          edges | boost::adaptors::reversed
      )
      {
        if (_mp->get_edge_property(out_edge).enabled)
        {
          _to_visit.push(std::make_tuple(out_edge, depth_ + 1));
        }
      }
    }
  }
}

forward_trace_iterator& forward_trace_iterator::operator++()
{
  if (_to_visit.empty())
  {
    _finished = true;
  }
  else
  {
    metaprogram::optional_edge_descriptor edge;
    int depth;
    std::tie(edge, depth) = _to_visit.top();
    _to_visit.pop();
    visit(edge, depth);
  }
  return *this;
}

const call_graph_node& forward_trace_iterator::operator*() const
{
  return _current;
}

bool forward_trace_iterator::operator==(const forward_trace_iterator& i_) const
{
  return _finished == i_._finished;
}

