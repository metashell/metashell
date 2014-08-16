
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

#include <cassert>
#include <algorithm>

#include <boost/range/adaptor/reversed.hpp>

namespace metashell {

metaprogram::metaprogram_state::metaprogram_state() {}

metaprogram::metaprogram_state::metaprogram_state(
    const metaprogram& trace)
{
  unsigned vertex_count = boost::num_vertices(trace.graph);
  if (vertex_count > 0) {
    discovered.resize(vertex_count, false);
    // 0 == <root> vertex
    vertex_stack.push(std::make_tuple(0, instantiation_kind()));  }
}

metaprogram::vertex_descriptor metaprogram::add_vertex(
  const std::string& element,
  const file_location& point_of_instantiation)
{
  vertex_descriptor vertex = boost::add_vertex(graph);

  template_vertex_property& vertex_property =
    boost::get(template_vertex_property_tag(), graph, vertex);

  vertex_property.name = element;
  vertex_property.point_of_instantiation = point_of_instantiation;

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

  boost::get(template_edge_property_tag(), graph, edge).kind = kind;
}

boost::optional<metaprogram::vertex_descriptor>
  metaprogram::find_vertex(const std::string& element) const
{
  vertex_iterator begin, end;
  std::tie(begin, end) = boost::vertices(graph);
  vertex_iterator it =
    std::find_if(begin, end,
      [&](vertex_descriptor vertex) {
        return boost::get(
          template_vertex_property_tag(), graph, vertex).name == element;
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

void metaprogram::start_metaprogram() {
  reset_metaprogram_state();
}

bool metaprogram::is_metaprogram_started() const {
  return !mp_state.vertex_stack.empty();
}

// Returns true when the program took it's last step (finished)
bool metaprogram::step_metaprogram() {
  assert(is_metaprogram_started());

  vertex_descriptor current_vertex;
  std::tie(current_vertex, std::ignore) = mp_state.vertex_stack.top();
  mp_state.vertex_stack.pop();

  if (!mp_state.discovered[current_vertex]) {
    mp_state.discovered[current_vertex] = true;

    for (edge_descriptor edge :
        boost::make_iterator_range(
          boost::out_edges(current_vertex, graph)) | boost::adaptors::reversed)
    {
      instantiation_kind next_kind =
        boost::get(template_edge_property_tag(), graph, edge).kind;

      mp_state.vertex_stack.push(
        std::make_tuple(
          boost::target(edge, graph), next_kind));
    }
  }
  return mp_state.vertex_stack.empty();
}

const metaprogram::graph_t& metaprogram::get_graph() const {
  return graph;
}

const metaprogram::metaprogram_state& metaprogram::get_state() const {
  return mp_state;
}

}

