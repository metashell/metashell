
#include "templight_trace.hpp"

#include <map>
#include <utility>
#include <iostream>
#include <cassert>

#include <boost/tuple/tuple.hpp> //for boost::tie
#include <boost/graph/graphviz.hpp>

namespace metashell {

templight_trace::vertex_descriptor templight_trace::add_vertex(
  const std::string& element,
  const file_location& point_of_instantiation)
{
  element_vertex_map_t::iterator pos;
  bool inserted;

  boost::tie(pos, inserted) = element_vertex_map.insert(
      std::make_pair(element, vertex_descriptor()));

  if (inserted) {
    vertex_descriptor vertex = boost::add_vertex(graph);
    pos->second = vertex;

    template_vertex_property& vertex_property =
      boost::get(template_vertex_property_tag(), graph, vertex);

    vertex_property.name = element;
    vertex_property.point_of_instantiation = point_of_instantiation;

    return vertex;
  }
  return pos->second;
}

void templight_trace::add_edge(
    vertex_descriptor from,
    vertex_descriptor to,
    instantiation_kind kind)
{
  edge_descriptor edge;
  bool inserted;

  boost::tie(edge, inserted) = boost::add_edge(from, to, graph);

  assert(inserted);

  boost::get(template_edge_property_tag(), graph, edge).kind = kind;
}

void templight_trace::print_graph(std::ostream& os) const {

  const_vertex_property_map_t vertex_map =
      boost::get(template_vertex_property_tag(), graph);

  const_edge_property_map_t edge_map =
      boost::get(template_edge_property_tag(), graph);

  os << "Verticies:\n";
  vertex_iterator vi, vi_end;
  for (boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi) {
    const template_vertex_property& vertex_property =
      boost::get(vertex_map, *vi);

    os << *vi << " : " <<
      vertex_property.name <<
      " instantiated from " <<
      vertex_property.point_of_instantiation << '\n';
  }

  os << "Edges:\n";
  edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
    os << boost::get(vertex_map, source(*ei, graph)).name <<
      " ---" << boost::get(edge_map, *ei).kind << "---> " <<
      boost::get(vertex_map, target(*ei, graph)).name << '\n';
  }
}

struct templight_trace::vertex_property_writer {
  vertex_property_writer(const templight_trace& trace) : trace(trace) {}

  void operator()(std::ostream& os, vertex_descriptor vertex) {
    os << "[label=\"" <<
      boost::get(template_vertex_property_tag(), trace.graph, vertex).name <<
      "\"]";
  }
private:
  const templight_trace& trace;
};

struct templight_trace::edge_property_writer {
  edge_property_writer(const templight_trace& trace) : trace(trace) {}

  void operator()(std::ostream& os, edge_descriptor edge) {
    os << "[label=\""<<
      boost::get(template_edge_property_tag(), trace.graph, edge).kind <<
      "\"]";
  }
private:
  const templight_trace& trace;
};

void templight_trace::print_graphviz(std::ostream& os) const {

  boost::write_graphviz(
      os, graph, vertex_property_writer(*this), edge_property_writer(*this));
}

std::ostream& operator<<(std::ostream& os, instantiation_kind kind) {
  switch (kind) {
    default:
        os << "UnknownKind"; break;
    case template_instantiation:
        os << "TemplateInstantiation"; break;
    case default_template_argument_instantiation:
        os << "DefaultTemplateArgumentInstantiation"; break;
    case default_function_argument_instantiation:
        os << "DefaultFunctionArgumentInstantiation"; break;
    case explicit_template_argument_substitution:
        os << "ExplicitTemplateArgumentSubstitution"; break;
    case deduced_template_argument_substitution:
        os << "DeducedTemplateArgumentSubstitution"; break;
    case prior_template_argument_substitution:
        os << "PriorTemplateArgumentSubstitution"; break;
    case default_template_argument_checking:
        os << "DefaultTemplateArgumentChecking"; break;
    case exception_spec_instantiation:
        os << "ExceptionSpecInstantiation"; break;
    case memoization:
        os << "Memoization"; break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const templight_trace& trace) {
  trace.print_graph(os);
  return os;
}

}

