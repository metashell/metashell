#ifndef METASHELL_DATA_METAPROGRAM_HPP
#define METASHELL_DATA_METAPROGRAM_HPP

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
#include <tuple>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/optional.hpp>

#include <metashell/data/backtrace.hpp>
#include <metashell/data/cpp_code.hpp>
#include <metashell/data/direction_t.hpp>
#include <metashell/data/event_kind.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/metaprogram_node.hpp>
#include <metashell/data/type_or_error.hpp>

namespace metashell
{
  namespace data
  {
    class metaprogram
    {
    public:
      enum class mode_t
      {
        normal,
        full,
        profile
      };

      // Creates empty metaprogram: single <root> vertex
      metaprogram(mode_t mode,
                  const cpp_code& root_name,
                  const file_location& root_source_location,
                  const type_or_error& evaluation_result);

      struct vertex_property_tag
      {
        typedef boost::vertex_property_tag kind;
      };
      struct edge_property_tag
      {
        typedef boost::edge_property_tag kind;
      };

      struct vertex_property
      {
        metaprogram_node node;
        file_location source_location;
      };
      struct edge_property
      {
        event_kind kind;
        file_location point_of_event;
        double begin_timestamp = 0.0;
        double time_taken = 0.0;
        bool enabled = true;
      };

      typedef boost::adjacency_list<
          boost::vecS,
          boost::vecS,
          boost::bidirectionalS,
          boost::property<vertex_property_tag, vertex_property>,
          boost::property<edge_property_tag, edge_property>>
          graph_t;

      typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
      typedef boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;

      typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;
      typedef boost::graph_traits<graph_t>::edge_iterator edge_iterator;
      typedef boost::graph_traits<graph_t>::in_edge_iterator in_edge_iterator;
      typedef boost::graph_traits<graph_t>::out_edge_iterator out_edge_iterator;

      typedef boost::graph_traits<graph_t>::vertices_size_type
          vertices_size_type;
      typedef boost::graph_traits<graph_t>::edges_size_type edges_size_type;
      typedef boost::graph_traits<graph_t>::degree_size_type degree_size_type;

      typedef boost::optional<vertex_descriptor> optional_vertex_descriptor;
      typedef boost::optional<edge_descriptor> optional_edge_descriptor;

      typedef std::vector<bool> discovered_t;
      typedef std::vector<optional_edge_descriptor> parent_edge_t;
      typedef std::stack<optional_edge_descriptor> edge_stack_t;

      struct state_t
      {
        discovered_t discovered;
        parent_edge_t parent_edge;
        edge_stack_t edge_stack;
      };

      struct step_rollback_t
      {
        optional_edge_descriptor popped_edge;
        optional_vertex_descriptor discovered_vertex;
        unsigned edge_stack_push_count = 0;
        boost::optional<optional_edge_descriptor> set_parent_edge;
      };

      typedef std::stack<step_rollback_t> state_history_t;

      vertex_descriptor add_vertex(const type& type,
                                   const file_location& source_location);

      edge_descriptor add_edge(vertex_descriptor from,
                               vertex_descriptor to,
                               event_kind kind,
                               const file_location& point_of_event,
                               double begin_timestamp);

      // Should be called after graph filtering is done
      void init_full_time_taken();

      bool is_empty() const;

      const type_or_error& get_evaluation_result() const;

      void reset_state();

      mode_t get_mode() const;

      bool is_at_endpoint(direction_t direction) const;
      bool is_finished() const;
      bool is_at_start() const;

      vertex_descriptor get_root_vertex() const;

      template <class P>
      void disable_edges_if(P pred);

      void step(direction_t direction);
      void step();
      void step_back();

      vertex_descriptor get_current_vertex() const;
      optional_edge_descriptor get_current_edge() const;
      frame get_current_frame() const;
      frame get_root_frame() const;
      backtrace get_backtrace() const;
      unsigned get_backtrace_length() const;

      unsigned get_traversal_count(vertex_descriptor vertex) const;

      const state_t& get_state() const;

      vertices_size_type get_num_vertices() const;
      edges_size_type get_num_edges() const;
      degree_size_type get_enabled_in_degree(vertex_descriptor vertex) const;
      degree_size_type get_enabled_out_degree(vertex_descriptor vertex) const;

      vertex_descriptor get_source(const edge_descriptor& edge) const;
      vertex_descriptor get_target(const edge_descriptor& edge) const;

      boost::iterator_range<in_edge_iterator>
      get_in_edges(vertex_descriptor vertex) const;
      boost::iterator_range<out_edge_iterator>
      get_out_edges(vertex_descriptor vertex) const;

      std::vector<edge_descriptor>
      get_filtered_out_edges(vertex_descriptor vertex) const;

      boost::iterator_range<vertex_iterator> get_vertices() const;
      boost::iterator_range<edge_iterator> get_edges() const;

      const vertex_property&
      get_vertex_property(vertex_descriptor vertex) const;
      const edge_property& get_edge_property(edge_descriptor edge) const;

      vertex_property& get_vertex_property(vertex_descriptor vertex);
      edge_property& get_edge_property(edge_descriptor edge);

      frame to_frame(const edge_descriptor& e_) const;

    private:
      typedef std::vector<boost::optional<unsigned>> traversal_counts_t;

      unsigned get_full_traversal_count(vertex_descriptor vertex) const;
      unsigned get_full_traversal_count_helper(
          vertex_descriptor vertex, traversal_counts_t& traversal_counts) const;

      graph_t graph;

      state_t state;
      state_history_t state_history;

      mode_t mode;

      // This should be generally 0
      vertex_descriptor root_vertex;

      type_or_error evaluation_result;

      // Time taken to run the full metaprogram (used to show % in profiling
      // data)
      double full_time_taken = 0.0;
    };

    template <class P>
    void metaprogram::disable_edges_if(P pred)
    {
      for (edge_descriptor edge : get_edges())
      {
        if (pred(edge))
        {
          get_edge_property(edge).enabled = false;
        }
      }
    }

    std::ostream& operator<<(std::ostream& os, metaprogram::mode_t mode);
  }
}

#endif
