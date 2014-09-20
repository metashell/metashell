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

#include <boost/optional.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <metashell/instantiation_kind.hpp>

namespace metashell {

class metaprogram {
public:

  // Creates empty metaprogram: single <root> vertex
  metaprogram();

  // Like the default constructor, but "step"-ed once
  static metaprogram create_empty_finished();

  static metaprogram create_from_xml_stream(std::istream& stream);
  static metaprogram create_from_xml_file(const std::string& file);
  static metaprogram create_from_xml_string(const std::string& string);

  struct vertex_property_tag {
    typedef boost::vertex_property_tag kind;
  };
  struct edge_property_tag {
    typedef boost::edge_property_tag kind;
  };

  struct vertex_property {
    std::string name;
  };
  struct edge_property {
    instantiation_kind kind;
  };

  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    boost::property<vertex_property_tag, vertex_property>,
    boost::property<edge_property_tag, edge_property>> graph_t;

  typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;

  typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;
  typedef boost::graph_traits<graph_t>::edge_iterator edge_iterator;
  typedef boost::graph_traits<graph_t>::in_edge_iterator in_edge_iterator;
  typedef boost::graph_traits<graph_t>::out_edge_iterator out_edge_iterator;

  typedef boost::graph_traits<graph_t>::vertices_size_type vertices_size_type;
  typedef boost::graph_traits<graph_t>::edges_size_type edges_size_type;

  typedef boost::optional<edge_descriptor> optional_edge_descriptor;

  typedef std::vector<bool> discovered_t;
  typedef std::vector<optional_edge_descriptor> parent_edge_t;
  typedef std::stack<optional_edge_descriptor> edge_stack_t;

  struct state_t {
    discovered_t discovered;
    parent_edge_t parent_edge;
    edge_stack_t edge_stack;
  };

  struct frame_t {
    frame_t();
    frame_t(vertex_descriptor vertex, edge_descriptor parent_edge);

    vertex_descriptor vertex;
    edge_descriptor parent_edge;
  };

  typedef std::vector<frame_t> backtrace_t;

  vertex_descriptor add_vertex(const std::string& element);

  edge_descriptor add_edge(
      vertex_descriptor from,
      vertex_descriptor to,
      instantiation_kind kind);

  void reset_state();
  bool is_finished() const;
  bool is_at_start() const;

  vertex_descriptor get_root_vertex() const;

  void step();

  vertex_descriptor get_current_vertex() const;
  frame_t get_current_frame() const;
  backtrace_t get_backtrace() const;

  const graph_t& get_graph() const;
  const state_t& get_state() const;

  vertices_size_type get_num_vertices() const;
  edges_size_type get_num_edges() const;

  const vertex_property& get_vertex_property(
      vertex_descriptor vertex) const;
  const edge_property& get_edge_property(
      edge_descriptor edge) const;

  vertex_property& get_vertex_property(
      vertex_descriptor vertex);
  edge_property& get_edge_property(
      edge_descriptor edge);

private:
  graph_t graph;

  state_t state;

  // This should be generally 0
  vertex_descriptor root_vertex;
};

}

#endif
