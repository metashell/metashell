
#include <map>
#include <stack>
#include <utility>
#include <iostream>
#include <cassert>
#include <algorithm>

#include <boost/tuple/tuple.hpp> //for boost::tie
#include <boost/graph/graphviz.hpp>

#include <metashell/templight_trace.hpp>

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

boost::optional<templight_trace::vertex_descriptor>
  templight_trace::find_vertex(const std::string& element) const
{
  element_vertex_map_t::const_iterator it =
    element_vertex_map.find(element);

  if (it == element_vertex_map.end()) {
    return boost::none;
  }

  return it->second;
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

struct templight_trace::property_writer {
  property_writer(const templight_trace& trace) : trace(trace) {}

  void operator()(std::ostream& os, vertex_descriptor vertex) {
    os << "[label=\"" <<
      boost::get(template_vertex_property_tag(), trace.graph, vertex).name <<
      "\"]";
  }

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
      os, graph, property_writer(*this), property_writer(*this));
}

struct templight_trace::is_memoziation_predicate {
  is_memoziation_predicate(const graph_t& graph) :
    graph(graph) {}

  bool operator()(const edge_descriptor& edge) const {
    return
      boost::get(template_edge_property_tag(), graph, edge).kind == memoization;
  }
private:
  const graph_t& graph;
};

template<class EdgeIterator, class GetEdges, class EdgeDirection>
void templight_trace::print_trace(
    const std::string& type, std::ostream& os,
    GetEdges get_edges, EdgeDirection edge_direction) const
{
  boost::optional<vertex_descriptor> opt_vertex =
    find_vertex(type);

  if (!opt_vertex) {
    os << "type \"" << type << "\" not found" << std::endl;
    return;
  }

  // -----
  // Customized DFS
  //   The algorithm only checks vertices which are reachable from type
  // ----
  std::vector<bool> discovered(boost::num_vertices(graph), false);

  // Second argument is depth
  typedef boost::tuple<
    vertex_descriptor, unsigned, instantiation_kind> stack_element;

  std::stack<stack_element> to_visit;
  // We don't care about the instantiation_kind for the source vertex
  to_visit.push(boost::make_tuple(*opt_vertex, 0, instantiation_kind()));

  bool first_iteration = true;
  while (!to_visit.empty()) {
    unsigned depth;
    vertex_descriptor vertex;
    instantiation_kind kind;
    boost::tie(vertex, depth, kind) = to_visit.top();
    to_visit.pop();

    os << std::string(2*depth, ' ') <<
      boost::get(template_vertex_property_tag(), graph, vertex).name;
    if (!first_iteration) { os << " (" <<  kind << ")"; }
    os << '\n';

    if (!discovered[vertex]) {
      discovered[vertex] = true;

      EdgeIterator begin, end;
      boost::tie(begin, end) = (this->*get_edges)(vertex);

      typedef std::vector<edge_descriptor> edges_t;
      edges_t edges(begin, end);

      // Partition Memozations to the back, so they get into the stack first
      std::stable_partition(
          edges.begin(), edges.end(), is_memoziation_predicate(graph));

      for (edges_t::iterator it = edges.begin(), end = edges.end();
         it != end; ++it)
      {
        instantiation_kind next_kind =
          boost::get(template_edge_property_tag(), graph, *it).kind;

        to_visit.push(
            boost::make_tuple((this->*edge_direction)(*it), depth+1, next_kind));
      }
    }
    first_iteration = false;
  }
}

std::pair<
  templight_trace::out_edge_iterator,
  templight_trace::out_edge_iterator
> templight_trace::get_out_edges(vertex_descriptor v) const {
  return boost::out_edges(v, graph);
}

std::pair<
  templight_trace::in_edge_iterator,
  templight_trace::in_edge_iterator
> templight_trace::get_in_edges(vertex_descriptor v) const {
  return boost::in_edges(v, graph);
}

templight_trace::vertex_descriptor templight_trace::get_source(
    edge_descriptor e) const
{
  return boost::source(e, graph);
}

templight_trace::vertex_descriptor templight_trace::get_target(
    edge_descriptor e) const
{
  return boost::target(e, graph);
}

void templight_trace::print_forwardtrace(
    const std::string& type,
    std::ostream& os) const
{
  print_trace<out_edge_iterator>(type, os,
      &templight_trace::get_out_edges, &templight_trace::get_target);
}

void templight_trace::print_backtrace(
    const std::string& type,
    std::ostream& os) const
{
  print_trace<in_edge_iterator>(type, os,
      &templight_trace::get_in_edges, &templight_trace::get_source);
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

