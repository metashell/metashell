#ifndef METASHELL_TEMPLIGHT_TRACE_HPP
#define METASHELL_TEMPLIGHT_TRACE_HPP

#include <string>
#include <vector>
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace metashell {

class templight_trace {
public:

  static templight_trace create_from_xml(const std::string& file);

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

  void print_graph(std::ostream& os = std::cout) const;

  //vertex names are currently stored reduntantly:
  // - in graph vertex_name property
  // - in element_vertex_map
  graph_t graph;

  element_vertex_map_t element_vertex_map;
};

}

#endif
