#ifndef METASHELL_TEMPLIGHT_TRACE_HPP
#define METASHELL_TEMPLIGHT_TRACE_HPP

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
#include <string>
#include <vector>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <just/console.hpp>

#include "file_location.hpp"

namespace metashell {

//Fwd:
class metadebugger_shell;

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
  void print_graphviz(std::ostream& os = std::cout) const;

  void print_forwardtrace(const metadebugger_shell& sh, const std::string& type) const;
  void print_full_forwardtrace(const metadebugger_shell& sh) const;
  void print_backtrace(const metadebugger_shell& sh, const std::string& type) const;
  void print_full_backtrace(const metadebugger_shell& sh) const;
  void print_current_frame(const metadebugger_shell& sh) const;

  void start_metaprogram();
  bool is_metaprogram_started() const;

  // Returns true when the program took it's last step (finished)
  bool step_metaprogram();

private:

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

  typedef std::pair<
        std::string::const_iterator,
        std::string::const_iterator
      > string_range;

  typedef std::vector<bool> discovered_t;

  struct metaprogram_state {

    typedef boost::tuple<
      vertex_descriptor,
      instantiation_kind
    > stack_element;

    typedef std::stack<stack_element> vertex_stack_t;

    metaprogram_state();
    explicit metaprogram_state(const templight_trace& trace);

    discovered_t discovered;
    vertex_stack_t vertex_stack;
  };

  void reset_metaprogram_state();

  vertex_descriptor add_vertex(
      const std::string& element,
      const file_location& point_of_instantiation);

  void add_edge(
      vertex_descriptor from,
      vertex_descriptor to,
      instantiation_kind kind);

  boost::optional<vertex_descriptor> find_vertex(
      const std::string& element) const;

  string_range find_type_emphasize(const std::string& type) const;

  void print_trace_content(
      const metadebugger_shell& sh,
      string_range range,
      string_range emphasize) const;

  void print_trace_graph(
      const metadebugger_shell& sh,
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      bool print_mark) const;

  void print_trace_line(
      const metadebugger_shell& sh,
      vertex_descriptor root_vertex,
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      const boost::optional<instantiation_kind>& kind,
      unsigned width) const;

  template<class EdgeIterator, class GetEdges, class EdgeDirection>
  void print_trace_visit(
      const metadebugger_shell& sh,
      vertex_descriptor vertex,
      discovered_t& discovered,
      GetEdges get_edges,
      EdgeDirection edge_direction,
      unsigned width) const;

  //vertex names are currently stored reduntantly:
  // - in graph vertex_name property
  // - in element_vertex_map
  graph_t graph;

  metaprogram_state mp_state;

  struct only_has_discovered_out_edge_predicate;

  friend struct templight_trace_builder;

  const static std::vector<just::console::color> colors;
};

std::ostream& operator<<(std::ostream& os, instantiation_kind kind);
std::ostream& operator<<(std::ostream& os, const templight_trace& trace);

}

#endif
