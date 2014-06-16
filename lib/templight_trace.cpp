
#include "templight_trace.hpp"

#include <map>
#include <iostream>
#include <utility>

#include <boost/tuple/tuple.hpp> //for boost::tie

namespace metashell {

templight_trace::vertex_descriptor templight_trace::add_vertex(
  const std::string& element)
{
  element_vertex_map_t::iterator pos;
  bool inserted;

  boost::tie(pos, inserted) = element_vertex_map.insert(
      std::make_pair(element, vertex_descriptor()));

  if (inserted) {
    vertex_descriptor vertex = boost::add_vertex(graph);
    pos->second = vertex;
    boost::put(boost::get(boost::vertex_name, graph), vertex, element);
    return vertex;
  }
  return pos->second;
}

void templight_trace::print_graph(std::ostream& os) const {

  boost::property_map<graph_t, boost::vertex_name_t>::const_type name_map =
    boost::get(boost::vertex_name, graph);

  os << "Verticies:\n";
  vertex_iterator vi, vi_end;
  for (boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi) {
    os << *vi << " : " << boost::get(name_map, *vi) << '\n';
  }

  os << "Edges:\n";
  edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
    os << boost::get(name_map, source(*ei, graph)) << " -> "
        << boost::get(name_map, target(*ei, graph)) << '\n';
  }
}

}

