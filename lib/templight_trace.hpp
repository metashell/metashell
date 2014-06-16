#ifndef METASHELL_TEMPLIGHT_TRACE_HPP
#define METASHELL_TEMPLIGHT_TRACE_HPP

#include <string>
#include <vector>

#include <boost/bimap.hpp>

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
    boost::bidirectionalS> graph_t;

  typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;
  typedef boost::bimap<std::string, vertex_t> name_vertex_map_t;

  graph_t graph;
  name_vertex_map_t name_vertex_map;

};

}

#endif
