#ifndef METASHELL_TEMPLIGHT_TRACE_HPP
#define METASHELL_TEMPLIGHT_TRACE_HPP

#include <string>
#include <vector>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace metashell {

enum instantiation_kind {
  template_instantiation,
  default_template_argument_instantiation,
  default_function_argument_instantiation,
  explicit_template_argument_substitution,
  deduced_template_argument_substitution,
  prior_template_argument_substitution,
  default_template_argument_checking,
  exception_spec_instantiation,
  memoization
};

class templight_trace {
public:

  static templight_trace create_from_xml(const std::string& file);

  void print_graph(std::ostream& os = std::cout) const;
private:
  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    boost::property<boost::vertex_name_t, std::string> > graph_t;

  typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<graph_t>::edge_iterator edge_iterator;
  typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;

  typedef std::map<std::string, vertex_descriptor> element_vertex_map_t;

  vertex_descriptor add_vertex(const std::string& element);

  //vertex names are currently stored reduntantly:
  // - in graph vertex_name property
  // - in element_vertex_map
  graph_t graph;

  element_vertex_map_t element_vertex_map;
};

std::ostream& operator<<(std::ostream& os, instantiation_kind kind);
std::ostream& operator<<(std::ostream& os, const templight_trace& trace);

}

#endif
