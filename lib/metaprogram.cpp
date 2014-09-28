
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

metaprogram::metaprogram(
    const std::string& root_name,
    const std::string& evaluation_result) :
  evaluation_result(evaluation_result)
{
  root_vertex = add_vertex(root_name);
  reset_state();
}

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
    instantiation_kind kind,
    const file_location& point_of_instantiation)
{
  edge_descriptor edge;
  bool inserted;
  std::tie(edge, inserted) = boost::add_edge(from, to, graph);

  assert(inserted);

  get_edge_property(edge).kind = kind;
  get_edge_property(edge).point_of_instantiation = point_of_instantiation;

  return edge;
}

const std::string& metaprogram::get_evaluation_result() const {
  return evaluation_result;
}

void metaprogram::reset_state() {
  unsigned vertex_count = get_num_vertices();
  assert(vertex_count > 0);

  state.discovered = discovered_t(vertex_count, false);
  state.parent_edge = parent_edge_t(vertex_count, boost::none);
  state.edge_stack = edge_stack_t();
  state.edge_stack.push(boost::none);

  state_history = state_history_t();
}

bool metaprogram::is_finished() const {
  return state.edge_stack.empty();
}

bool metaprogram::is_at_start() const {
  return state.edge_stack.size() == 1 && !state.edge_stack.top();
}

metaprogram::vertex_descriptor metaprogram::get_root_vertex() const {
  return root_vertex;
}

void metaprogram::step() {
  assert(!is_finished());
  step_rollback_t rollback;

  vertex_descriptor current_vertex = get_current_vertex();
  rollback.popped_edge = state.edge_stack.top();
  state.edge_stack.pop();

  if (!state.discovered[current_vertex]) {
    state.discovered[current_vertex] = true;
    rollback.discovered_vertex = current_vertex;

    auto reverse_edge_range =
      get_out_edges(current_vertex) |boost::adaptors::reversed;

    rollback.edge_stack_push_count = 0;
    for (edge_descriptor edge : reverse_edge_range) {
      if (get_edge_property(edge).enabled) {
        state.edge_stack.push(edge);
        ++rollback.edge_stack_push_count;
      }
    }
  }

  if (!state.edge_stack.empty()) {
    assert(state.edge_stack.top());
    rollback.set_parent_edge = state.parent_edge[get_current_vertex()];
    state.parent_edge[get_current_vertex()] = *state.edge_stack.top();
  }
  state_history.push(rollback);
}

void metaprogram::step_back() {
  assert(!is_at_start());
  assert(!state_history.empty());

  const step_rollback_t& rollback = state_history.top();

  if (rollback.set_parent_edge) {
    state.parent_edge[get_current_vertex()] = *rollback.set_parent_edge;
  }

  for (unsigned i = 0; i < rollback.edge_stack_push_count; ++i) {
    state.edge_stack.pop();
  }

  if (rollback.discovered_vertex) {
    state.discovered[*rollback.discovered_vertex] = false;
  }

  state.edge_stack.push(rollback.popped_edge);

  state_history.pop();
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

metaprogram::vertex_descriptor metaprogram::get_source(
    const edge_descriptor& edge) const
{
  return boost::source(edge, graph);
}

metaprogram::vertex_descriptor metaprogram::get_target(
    const edge_descriptor& edge) const
{
  return boost::target(edge, graph);
}

boost::iterator_range<metaprogram::in_edge_iterator>
metaprogram::get_in_edges(vertex_descriptor vertex) const {
  return boost::in_edges(vertex, graph);
}

boost::iterator_range<metaprogram::out_edge_iterator>
metaprogram::get_out_edges(vertex_descriptor vertex) const {
  return boost::out_edges(vertex, graph);
}

boost::iterator_range<metaprogram::vertex_iterator>
metaprogram::get_vertices() const {
  return boost::vertices(graph);
}

boost::iterator_range<metaprogram::edge_iterator>
metaprogram::get_edges() const {
  return boost::edges(graph);
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
  return get_target(*edge);
}

metaprogram::optional_edge_descriptor metaprogram::get_current_edge() const {
  return state.edge_stack.top();
}

metaprogram::edge_descriptor metaprogram::get_current_frame() const {
  assert(!is_finished());

  vertex_descriptor current_vertex = get_current_vertex();

  assert(state.parent_edge[current_vertex]);

  return *state.parent_edge[current_vertex];
}

metaprogram::backtrace_t metaprogram::get_backtrace() const {
  assert(!is_finished());

  backtrace_t backtrace;

  for (vertex_descriptor current_vertex = get_current_vertex();
      current_vertex != get_root_vertex(); )
  {
    assert(state.parent_edge[current_vertex]);

    edge_descriptor parent_edge = *state.parent_edge[current_vertex];
    backtrace.push_back(parent_edge);

    current_vertex = get_source(parent_edge);
  }

  return backtrace;
}

}

