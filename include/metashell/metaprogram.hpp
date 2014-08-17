#ifndef METASHELL_METAPROGRAM_HPP
#define METASHELL_METAPROGRAM_HPP

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

#include <stack>
#include <tuple>
#include <string>
#include <vector>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <metashell/file_location.hpp>
#include <metashell/instantiation_kind.hpp>

namespace metashell {

class metaprogram {
public:
  static metaprogram create_from_xml_stream(std::istream& stream);
  static metaprogram create_from_xml_file(const std::string& file);
  static metaprogram create_from_xml_string(const std::string& string);

  struct template_vertex_property_tag {
    typedef boost::vertex_property_tag kind;
  };
  struct template_edge_property_tag {
    typedef boost::edge_property_tag kind;
  };

  struct template_vertex_property {
    std::string name;
    file_location point_of_instantiation;
  };
  struct template_edge_property {
    instantiation_kind kind;
  };

  typedef boost::property<
    template_vertex_property_tag,
    template_vertex_property> vertex_property;
  typedef boost::property<
    template_edge_property_tag,
    template_edge_property> edge_property;

  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    vertex_property,
    edge_property> graph_t;

  typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;

  typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;
  typedef boost::graph_traits<graph_t>::edge_iterator edge_iterator;
  typedef boost::graph_traits<graph_t>::in_edge_iterator in_edge_iterator;
  typedef boost::graph_traits<graph_t>::out_edge_iterator out_edge_iterator;

  typedef boost::graph_traits<graph_t>::vertices_size_type vertices_size_type;
  typedef boost::graph_traits<graph_t>::edges_size_type edges_size_type;

  typedef boost::property_map<
      graph_t,
      template_vertex_property_tag>::type vertex_property_map_t;

  typedef boost::property_map<
      graph_t,
      template_vertex_property_tag>::const_type const_vertex_property_map_t;

  typedef boost::property_map<
      graph_t,
      template_edge_property_tag>::type edge_property_map_t;

  typedef boost::property_map<
      graph_t,
      template_edge_property_tag>::const_type const_edge_property_map_t;

  typedef std::vector<bool> discovered_t;
  typedef std::vector<boost::optional<edge_descriptor>> parent_edge_t;

  struct metaprogram_state {

    typedef std::stack<vertex_descriptor> vertex_stack_t;

    metaprogram_state();
    explicit metaprogram_state(const metaprogram& mp);

    discovered_t discovered;
    parent_edge_t parent_edge;
    vertex_stack_t vertex_stack;
  };

  struct frame {
    frame();
    frame(vertex_descriptor vertex, edge_descriptor parent_edge);

    vertex_descriptor vertex;
    edge_descriptor parent_edge;
  };

  typedef std::vector<frame> back_trace_t;

  vertex_descriptor add_vertex(
      const std::string& element,
      const file_location& point_of_instantiation);

  void add_edge(
      vertex_descriptor from,
      vertex_descriptor to,
      instantiation_kind kind);

  boost::optional<vertex_descriptor> find_vertex(
      const std::string& element) const;

  void reset_metaprogram_state();
  void start_metaprogram();
  bool is_metaprogram_started() const;

  // Returns true when the program took it's last step (finished)
  bool step_metaprogram();

  frame get_current_frame() const;
  back_trace_t get_back_trace() const;

  const graph_t& get_graph() const;
  const metaprogram_state& get_state() const;

  vertices_size_type get_num_vertices() const;
  edges_size_type get_num_edges() const;

  const template_vertex_property& get_vertex_property(
      vertex_descriptor vertex) const;
  const template_edge_property& get_edge_property(
      edge_descriptor edge) const;

  template_vertex_property& get_vertex_property(
      vertex_descriptor vertex);
  template_edge_property& get_edge_property(
      edge_descriptor edge);

private:
  graph_t graph;

  metaprogram_state mp_state;
};

}

#endif
