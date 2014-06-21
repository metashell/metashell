
#include "templight_trace.hpp"

#include <map>
#include <iostream>
#include <utility>

#include <boost/tuple/tuple.hpp> //for boost::tie
#include <boost/graph/graphviz.hpp>

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

void templight_trace::add_edge(vertex_descriptor from, vertex_descriptor to) {
  boost::add_edge(from, to, graph);
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

void templight_trace::print_graphviz(std::ostream& os) const {
  boost::write_graphviz(os, graph);
}

std::ostream& operator<<(std::ostream& os, instantiation_kind kind) {
  switch (kind) {
    default: os << "UnknownKind"; break;
    case template_instantiation: os << "TemplateInstantiation"; break;
    case default_template_argument_instantiation: os << "DefaultTemplateArgumentInstantiation"; break;
    case default_function_argument_instantiation: os << "DefaultFunctionArgumentInstantiation"; break;
    case explicit_template_argument_substitution: os << "ExplicitTemplateArgumentSubstitution"; break;
    case deduced_template_argument_substitution: os << "DeducedTemplateArgumentSubstitution"; break;
    case prior_template_argument_substitution: os << "PriorTemplateArgumentSubstitution"; break;
    case default_template_argument_checking: os << "DefaultTemplateArgumentChecking"; break;
    case exception_spec_instantiation: os << "ExceptionSpecInstantiation"; break;
    case memoization: os << "Memoization"; break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const templight_trace& trace) {
  trace.print_graph(os);
  return os;
}

}

