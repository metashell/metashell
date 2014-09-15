
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/metaprogram.hpp>

#include <tuple>
#include <cassert>
#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace metashell {

metaprogram::metaprogram() {
  root_vertex = add_vertex("<root>");
  reset_state();
}

metaprogram metaprogram::create_empty_finished() {
  metaprogram mp;

  mp.step();

  return mp;
}

metaprogram::frame_t::frame_t() : vertex(), parent_edge() {}

metaprogram::frame_t::frame_t(
    vertex_descriptor vertex, edge_descriptor parent_edge) :
  vertex(vertex), parent_edge(parent_edge) {}

metaprogram::vertex_descriptor metaprogram::add_vertex(
  const std::string& element)
{
  vertex_descriptor vertex = boost::add_vertex(graph);

  assert(state.discovered.size() == vertex);
  assert(state.parent_edge.size() == vertex);

  state.discovered.push_back(false);
  state.parent_edge.push_back(boost::none);

  get_vertex_property(vertex).name = element;

  return vertex;
}

metaprogram::edge_descriptor metaprogram::add_edge(
    vertex_descriptor from,
    vertex_descriptor to,
    instantiation_kind kind)
{
  edge_descriptor edge;
  bool inserted;
  std::tie(edge, inserted) = boost::add_edge(from, to, graph);

  assert(inserted);

  get_edge_property(edge).kind = kind;

  return edge;
}

void metaprogram::reset_state() {
  unsigned vertex_count = get_num_vertices();
  assert(vertex_count > 0);

  state.discovered = discovered_t(vertex_count, false);
  state.parent_edge = parent_edge_t(vertex_count, boost::none);
  state.edge_stack = edge_stack_t();
  state.edge_stack.push(boost::none);
}

bool metaprogram::is_finished() const {
  return state.edge_stack.empty();
}

metaprogram::vertex_descriptor metaprogram::get_root_vertex() const {
  return root_vertex;
}

void metaprogram::step() {
  assert(!is_finished());

  vertex_descriptor current_vertex = get_current_vertex();
  state.edge_stack.pop();

  if (!state.discovered[current_vertex]) {
    state.discovered[current_vertex] = true;

    auto reverse_edge_range = boost::make_iterator_range(
          boost::out_edges(current_vertex, graph)) | boost::adaptors::reversed;

    for (edge_descriptor edge : reverse_edge_range) {
      state.edge_stack.push(edge);
    }
  }

  if (!state.edge_stack.empty()) {
    assert(state.edge_stack.top());
    state.parent_edge[get_current_vertex()] = *state.edge_stack.top();
  }
}

const metaprogram::graph_t& metaprogram::get_graph() const {
  return graph;
}

const metaprogram::state_t& metaprogram::get_state() const {
  return state;
}

metaprogram::vertices_size_type metaprogram::get_num_vertices() const {
  return boost::num_vertices(graph);
}

metaprogram::edges_size_type metaprogram::get_num_edges() const {
  return boost::num_edges(graph);
}

const metaprogram::vertex_property& metaprogram::get_vertex_property(
    vertex_descriptor vertex) const
{
  return boost::get(vertex_property_tag(), graph, vertex);
}

const metaprogram::edge_property& metaprogram::get_edge_property(
    edge_descriptor edge) const
{
  return boost::get(edge_property_tag(), graph, edge);
}

metaprogram::vertex_property& metaprogram::get_vertex_property(
    vertex_descriptor vertex)
{
  return boost::get(vertex_property_tag(), graph, vertex);
}

metaprogram::edge_property& metaprogram::get_edge_property(
    edge_descriptor edge)
{
  return boost::get(edge_property_tag(), graph, edge);
}

metaprogram::vertex_descriptor metaprogram::get_current_vertex() const {
  assert(!is_finished());

  const optional_edge_descriptor& edge = state.edge_stack.top();
  if (!edge) {
    return get_root_vertex();
  }
  return boost::target(*edge, graph);
}

metaprogram::frame_t metaprogram::get_current_frame() const {
  assert(!is_finished());

  vertex_descriptor current_vertex = get_current_vertex();

  assert(state.parent_edge[current_vertex]);

  return frame_t(
      current_vertex,
      *state.parent_edge[current_vertex]);
}

metaprogram::backtrace_t metaprogram::get_backtrace() const {
  assert(!is_finished());

  vertex_descriptor current_vertex = get_current_vertex();

  backtrace_t backtrace;

  while (current_vertex != get_root_vertex()) {
    assert(state.parent_edge[current_vertex]);

    edge_descriptor parent_edge = *state.parent_edge[current_vertex];
    backtrace.push_back(
        frame_t(
          current_vertex,
          parent_edge
        )
      );

    current_vertex = boost::source(parent_edge, graph);
  }
  std::reverse(backtrace.begin(), backtrace.end());

  return backtrace;
}

}

