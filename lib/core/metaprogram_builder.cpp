
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

#include <metashell/metaprogram_builder.hpp>

#include <metashell/exception.hpp>

namespace metashell {

metaprogram_builder::metaprogram_builder(
    metaprogram::mode_t mode,
    const std::string& root_name,
    const data::type_or_error& evaluation_result) :
  mp(mode, root_name, evaluation_result)
{}

void metaprogram_builder::handle_template_begin(
  data::instantiation_kind kind,
  const std::string& context,
  const data::file_location& point_of_instantiation,
  double timestamp)
{
  vertex_descriptor vertex = add_vertex(context);
  vertex_descriptor top_vertex = edge_stack.empty() ?
    mp.get_root_vertex() : mp.get_target(edge_stack.top());

  auto edge =
    mp.add_edge(top_vertex, vertex, kind, point_of_instantiation, timestamp);
  edge_stack.push(edge);
}

void metaprogram_builder::handle_template_end(double timestamp) {
  if (edge_stack.empty()) {
    throw exception(
        "Mismatched Templight TemplateBegin and TemplateEnd events");
  }
  auto& ep = mp.get_edge_property(edge_stack.top());
  ep.time_taken = timestamp - ep.begin_timestamp;

  edge_stack.pop();
}

const metaprogram& metaprogram_builder::get_metaprogram() const {
  if (!edge_stack.empty()) {
    throw exception(
        "Some Templight TemplateEnd events are missing");
  }
  return mp;
}

metaprogram_builder::vertex_descriptor metaprogram_builder::add_vertex(
    const std::string& context)
{
  element_vertex_map_t::iterator pos;
  bool inserted;

  std::tie(pos, inserted) = element_vertex_map.insert(
      std::make_pair(context, vertex_descriptor()));

  if (inserted) {
    pos->second = mp.add_vertex(context);
  }
  return pos->second;
}

}
