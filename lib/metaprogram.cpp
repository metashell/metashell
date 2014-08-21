
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

#include "exception.hpp"

namespace metashell {

metaprogram::metaprogram() {
  root_vertex = add_vertex("<root>");
  reset_metaprogram_state();
}

metaprogram::metaprogram_state::metaprogram_state() {}

metaprogram::metaprogram_state::metaprogram_state(
    const metaprogram& mp)
{
  unsigned vertex_count = boost::num_vertices(mp.graph);
  assert(vertex_count > 0);

  discovered.resize(vertex_count, false);
  parent_edge.resize(vertex_count, boost::none);
  vertex_stack.push(mp.get_root_vertex());
}

metaprogram::frame::frame() : vertex(), parent_edge() {}

metaprogram::frame::frame(
    vertex_descriptor vertex, edge_descriptor parent_edge) :
  vertex(vertex), parent_edge(parent_edge) {}

metaprogram::vertex_descriptor metaprogram::add_vertex(
  const std::string& element)
{
  vertex_descriptor vertex = boost::add_vertex(graph);

  // TODO resizing after every add_vertex is neither
  // efficient nor nice
  mp_state.discovered.resize(vertex+1, false);
  mp_state.parent_edge.resize(vertex+1, boost::none);

  get_vertex_property(vertex).name = element;

  return vertex;
}

void metaprogram::add_edge(
    vertex_descriptor from,
    vertex_descriptor to,
    instantiation_kind kind)
{
  edge_descriptor edge;
  bool inserted;

  std::tie(edge, inserted) = boost::add_edge(from, to, graph);

  assert(inserted);

  get_edge_property(edge).kind = kind;
}

boost::optional<metaprogram::vertex_descriptor>
  metaprogram::find_vertex(const std::string& element) const
{
  vertex_iterator begin, end;
  std::tie(begin, end) = boost::vertices(graph);
  vertex_iterator it =
    std::find_if(begin, end,
      [&](vertex_descriptor vertex) {
        return get_vertex_property(vertex).name == element;
      }
    );
  if (it != end) {
    return *it;
  }
  return boost::none;
}

void metaprogram::reset_metaprogram_state() {
  mp_state = metaprogram_state(*this);
}

bool metaprogram::is_metaprogram_finished() const {
  return mp_state.vertex_stack.empty();
}

metaprogram::vertex_descriptor metaprogram::get_root_vertex() const {
  return root_vertex;
}

// Returns true when the program took it's last step (finished)
bool metaprogram::step_metaprogram() {
  assert(!is_metaprogram_finished());

  vertex_descriptor current_vertex = mp_state.vertex_stack.top();
  mp_state.vertex_stack.pop();

  if (!mp_state.discovered[current_vertex]) {
    mp_state.discovered[current_vertex] = true;

    auto reverse_edge_range = boost::make_iterator_range(
          boost::out_edges(current_vertex, graph)) | boost::adaptors::reversed;

    for (edge_descriptor edge : reverse_edge_range) {
      vertex_descriptor target = boost::target(edge, graph);

      mp_state.parent_edge[target] = edge;
      mp_state.vertex_stack.push(target);
    }
  }
  return is_metaprogram_finished();
}

const metaprogram::graph_t& metaprogram::get_graph() const {
  return graph;
}

const metaprogram::metaprogram_state& metaprogram::get_state() const {
  return mp_state;
}

metaprogram::vertices_size_type metaprogram::get_num_vertices() const {
  return boost::num_vertices(graph);
}

metaprogram::edges_size_type metaprogram::get_num_edges() const {
  return boost::num_edges(graph);
}

const metaprogram::template_vertex_property& metaprogram::get_vertex_property(
    vertex_descriptor vertex) const
{
  return boost::get(template_vertex_property_tag(), graph, vertex);
}

const metaprogram::template_edge_property& metaprogram::get_edge_property(
    edge_descriptor edge) const
{
  return boost::get(template_edge_property_tag(), graph, edge);
}

metaprogram::template_vertex_property& metaprogram::get_vertex_property(
    vertex_descriptor vertex)
{
  return boost::get(template_vertex_property_tag(), graph, vertex);
}

metaprogram::template_edge_property& metaprogram::get_edge_property(
    edge_descriptor edge)
{
  return boost::get(template_edge_property_tag(), graph, edge);
}

metaprogram::vertex_descriptor metaprogram::get_current_vertex() const {
  assert(!is_metaprogram_finished());
  return mp_state.vertex_stack.top();
}

metaprogram::frame metaprogram::get_current_frame() const {
  assert(!is_metaprogram_finished());

  vertex_descriptor current_vertex = mp_state.vertex_stack.top();

  assert(mp_state.parent_edge[current_vertex]);

  return frame(
      current_vertex,
      *mp_state.parent_edge[current_vertex]);
}

metaprogram::backtrace_t metaprogram::get_backtrace() const {
  assert(!is_metaprogram_finished());

  vertex_descriptor current_vertex = mp_state.vertex_stack.top();

  backtrace_t backtrace;

  while (current_vertex != get_root_vertex()) {
    assert(mp_state.parent_edge[current_vertex]);

    edge_descriptor parent_edge = *mp_state.parent_edge[current_vertex];
    backtrace.push_back(
        frame(
          current_vertex,
          parent_edge
        )
      );

    current_vertex = boost::source(parent_edge, graph);
  }

  return backtrace;
}

}

