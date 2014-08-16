
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

#include <map>
#include <deque>
#include <utility>
#include <iostream>
#include <cassert>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/graphviz.hpp>

#include <metashell/templight_trace.hpp>
#include <metashell/metadebugger_shell.hpp>

namespace metashell {

void templight_trace::print_graph(std::ostream& os) const {

  const_vertex_property_map_t vertex_map =
      boost::get(template_vertex_property_tag(), mp.get_graph());

  const_edge_property_map_t edge_map =
      boost::get(template_edge_property_tag(), mp.get_graph());

  os << "Verticies:\n";
  BOOST_FOREACH(vertex_descriptor vertex, boost::vertices(mp.get_graph())) {
    const template_vertex_property& vertex_property =
      boost::get(vertex_map, vertex);

    os << vertex << " : " <<
      vertex_property.name <<
      " instantiated from " <<
      vertex_property.point_of_instantiation << '\n';
  }

  os << "Edges:\n";
  BOOST_FOREACH(const edge_descriptor& edge, boost::edges(mp.get_graph())) {
    os << boost::get(vertex_map, source(edge, mp.get_graph())).name <<
      " ---" << boost::get(edge_map, edge).kind << "---> " <<
      boost::get(vertex_map, target(edge, mp.get_graph())).name << '\n';
  }
}

void templight_trace::print_graphviz(std::ostream& os) const {

  boost::write_graphviz(
      os, mp.get_graph(),
      [this](std::ostream& os, vertex_descriptor vertex) {
        os << "[label=\"" <<
          boost::get(template_vertex_property_tag(), mp.get_graph(), vertex).name <<
          "\"]";
      },
      [this](std::ostream& os, edge_descriptor edge) {
        os << "[label=\""<<
          boost::get(template_edge_property_tag(), mp.get_graph(), edge).kind <<
          "\"]";
      }
  );
}

void templight_trace::set_metaprogram(const metaprogram& new_mp) {
  mp = new_mp;
}

metaprogram& templight_trace::get_metaprogram() {
  return mp;
}

const metaprogram& templight_trace::get_metaprogram() const {
  return mp;
}

std::ostream& operator<<(std::ostream& os, const templight_trace& trace) {
  trace.print_graph(os);
  return os;
}

}

