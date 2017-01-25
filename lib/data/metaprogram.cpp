
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

#include <metashell/data/metaprogram.hpp>

#include <algorithm>
#include <cassert>
#include <tuple>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/reverse.hpp>
#include <boost/range/algorithm/sort.hpp>

namespace metashell
{
  namespace data
  {
    metaprogram::metaprogram(mode_t mode,
                             const std::string& root_name,
                             const file_location& root_source_location,
                             const type_or_error& evaluation_result)
      : mode(mode), evaluation_result(evaluation_result)
    {
      root_vertex = add_vertex(type(root_name), root_source_location);
      reset_state();
    }

    metaprogram::vertex_descriptor
    metaprogram::add_vertex(const type& type,
                            const file_location& source_location)
    {
      vertex_descriptor vertex = boost::add_vertex(graph);

      assert(state.discovered.size() == vertex);
      assert(state.parent_edge.size() == vertex);

      state.discovered.push_back(false);
      state.parent_edge.push_back(boost::none);

      auto& vertex_property = get_vertex_property(vertex);

      vertex_property.node = type;
      vertex_property.source_location = source_location;

      return vertex;
    }

    metaprogram::edge_descriptor
    metaprogram::add_edge(vertex_descriptor from,
                          vertex_descriptor to,
                          event_kind kind,
                          const file_location& point_of_event,
                          double begin_timestamp)
    {
      edge_descriptor edge;
      bool inserted;
      std::tie(edge, inserted) = boost::add_edge(from, to, graph);

      assert(inserted);

      auto& ep = get_edge_property(edge);

      ep.kind = kind;
      ep.point_of_event = point_of_event;
      ep.begin_timestamp = begin_timestamp;

      return edge;
    }

    void metaprogram::init_full_time_taken()
    {
      boost::optional<double> first_time_stamp;
      boost::optional<double> last_time_stamp;
      for (const auto& edge : get_edges())
      {
        const auto& ep = get_edge_property(edge);
        if (!ep.enabled)
        {
          continue;
        }
        if (!first_time_stamp || *first_time_stamp > ep.begin_timestamp)
        {
          first_time_stamp = ep.begin_timestamp;
        }
        if (!last_time_stamp ||
            *last_time_stamp < ep.begin_timestamp + ep.time_taken)
        {
          last_time_stamp = ep.begin_timestamp + ep.time_taken;
        }
      }
      if (!first_time_stamp || !last_time_stamp)
      {
        full_time_taken = 0.0;
      }
      else
      {
        full_time_taken = *last_time_stamp - *first_time_stamp;
      }
    }

    bool metaprogram::is_empty() const
    {
      return get_num_vertices() == 1; // 1 is the <root> vertex
    }

    const type_or_error& metaprogram::get_evaluation_result() const
    {
      return evaluation_result;
    }

    void metaprogram::reset_state()
    {
      unsigned vertex_count = get_num_vertices();
      assert(vertex_count > 0);

      state.discovered = discovered_t(vertex_count, false);
      state.parent_edge = parent_edge_t(vertex_count, boost::none);
      state.edge_stack = edge_stack_t();
      state.edge_stack.push(boost::none);

      state_history = state_history_t();
    }

    metaprogram::mode_t metaprogram::get_mode() const { return mode; }

    bool metaprogram::is_at_endpoint(direction_t direction) const
    {
      if (direction == direction_t::forward)
      {
        return is_finished();
      }
      else if (direction == direction_t::backwards)
      {
        return is_at_start();
      }
      else
      {
        assert(false);
        return true;
      }
    }

    bool metaprogram::is_finished() const
    {
      if (evaluation_result.is_type() || evaluation_result.is_none())
      {
        return state.edge_stack.empty();
      }

      // In case the evaluation_result is an error, we want to stop one step
      // before
      // the DFS is finished.
      //
      // Emulating a step() to see if we would get to a regular in_finished()
      // state
      // if we would've called a step()
      if (state.edge_stack.size() > 1)
      {
        return false;
      }
      vertex_descriptor current_vertex = get_current_vertex();
      if (state.discovered[current_vertex])
      {
        return true;
      }
      for (edge_descriptor edge : get_out_edges(current_vertex))
      {
        if (get_edge_property(edge).enabled)
        {
          return false;
        }
      }
      return true;
    }

    bool metaprogram::is_at_start() const
    {
      return state.edge_stack.size() == 1 && !state.edge_stack.top();
    }

    metaprogram::vertex_descriptor metaprogram::get_root_vertex() const
    {
      return root_vertex;
    }

    void metaprogram::step(direction_t direction)
    {
      if (direction == direction_t::forward)
      {
        step();
      }
      else if (direction == direction_t::backwards)
      {
        step_back();
      }
      else
      {
        assert(false);
      }
    }

    void metaprogram::step()
    {
      assert(!is_finished());
      step_rollback_t rollback;

      vertex_descriptor current_vertex = get_current_vertex();
      rollback.popped_edge = state.edge_stack.top();
      state.edge_stack.pop();

      if (!state.discovered[current_vertex])
      {
        if (get_mode() != mode_t::full)
        {
          state.discovered[current_vertex] = true;
          rollback.discovered_vertex = current_vertex;
        }

        auto edges = get_filtered_out_edges(current_vertex);

        rollback.edge_stack_push_count = edges.size();
        for (edge_descriptor edge : edges)
        {
          state.edge_stack.push(edge);
        }
      }

      if (!state.edge_stack.empty())
      {
        assert(state.edge_stack.top());
        rollback.set_parent_edge = state.parent_edge[get_current_vertex()];
        state.parent_edge[get_current_vertex()] = *state.edge_stack.top();
      }
      state_history.push(rollback);
    }

    void metaprogram::step_back()
    {
      assert(!is_at_start());
      assert(!state_history.empty());

      const step_rollback_t& rollback = state_history.top();

      if (rollback.set_parent_edge)
      {
        state.parent_edge[get_current_vertex()] = *rollback.set_parent_edge;
      }

      for (unsigned i = 0; i < rollback.edge_stack_push_count; ++i)
      {
        state.edge_stack.pop();
      }

      if (rollback.discovered_vertex)
      {
        state.discovered[*rollback.discovered_vertex] = false;
      }

      state.edge_stack.push(rollback.popped_edge);

      state_history.pop();
    }

    const metaprogram::state_t& metaprogram::get_state() const { return state; }

    metaprogram::vertices_size_type metaprogram::get_num_vertices() const
    {
      return boost::num_vertices(graph);
    }

    metaprogram::edges_size_type metaprogram::get_num_edges() const
    {
      return boost::num_edges(graph);
    }

    metaprogram::degree_size_type
    metaprogram::get_enabled_in_degree(vertex_descriptor vertex) const
    {
      degree_size_type result = 0;
      for (edge_descriptor edge : get_in_edges(vertex))
      {
        if (get_edge_property(edge).enabled)
        {
          ++result;
        }
      }
      return result;
    }

    metaprogram::degree_size_type
    metaprogram::get_enabled_out_degree(vertex_descriptor vertex) const
    {
      degree_size_type result = 0;
      for (edge_descriptor edge : get_out_edges(vertex))
      {
        if (get_edge_property(edge).enabled)
        {
          ++result;
        }
      }
      return result;
    }

    metaprogram::vertex_descriptor
    metaprogram::get_source(const edge_descriptor& edge) const
    {
      return boost::source(edge, graph);
    }

    metaprogram::vertex_descriptor
    metaprogram::get_target(const edge_descriptor& edge) const
    {
      return boost::target(edge, graph);
    }

    boost::iterator_range<metaprogram::in_edge_iterator>
    metaprogram::get_in_edges(vertex_descriptor vertex) const
    {
      return boost::in_edges(vertex, graph);
    }

    boost::iterator_range<metaprogram::out_edge_iterator>
    metaprogram::get_out_edges(vertex_descriptor vertex) const
    {
      return boost::out_edges(vertex, graph);
    }

    std::vector<metaprogram::edge_descriptor>
    metaprogram::get_filtered_out_edges(vertex_descriptor vertex) const
    {
      using boost::adaptors::filtered;
      using boost::range::sort;
      using boost::range::reverse;
      using boost::copy_range;

      auto is_enabled = [this](edge_descriptor edge) {
        return get_edge_property(edge).enabled;
      };

      auto edges = copy_range<std::vector<edge_descriptor>>(
          get_out_edges(vertex) | filtered(is_enabled));

      if (get_mode() == mode_t::profile)
      {
        // Traverse the eges which took a long time first
        auto time_order = [this](edge_descriptor lhs, edge_descriptor rhs) {
          return get_edge_property(lhs).time_taken >
                 get_edge_property(rhs).time_taken;
        };
        sort(edges, time_order);
      }
      // Reverse iteration, so types that got instantiated first
      // get on the top of the stack on the caller side
      reverse(edges);

      return edges;
    }

    boost::iterator_range<metaprogram::vertex_iterator>
    metaprogram::get_vertices() const
    {
      return boost::vertices(graph);
    }

    boost::iterator_range<metaprogram::edge_iterator>
    metaprogram::get_edges() const
    {
      return boost::edges(graph);
    }

    const metaprogram::vertex_property&
    metaprogram::get_vertex_property(vertex_descriptor vertex) const
    {
      return boost::get(vertex_property_tag(), graph, vertex);
    }

    const metaprogram::edge_property&
    metaprogram::get_edge_property(edge_descriptor edge) const
    {
      return boost::get(edge_property_tag(), graph, edge);
    }

    metaprogram::vertex_property&
    metaprogram::get_vertex_property(vertex_descriptor vertex)
    {
      return boost::get(vertex_property_tag(), graph, vertex);
    }

    metaprogram::edge_property&
    metaprogram::get_edge_property(edge_descriptor edge)
    {
      return boost::get(edge_property_tag(), graph, edge);
    }

    metaprogram::vertex_descriptor metaprogram::get_current_vertex() const
    {
      assert(!state.edge_stack.empty());

      const optional_edge_descriptor& edge = state.edge_stack.top();
      if (!edge)
      {
        return get_root_vertex();
      }
      return get_target(*edge);
    }

    metaprogram::optional_edge_descriptor metaprogram::get_current_edge() const
    {
      assert(!is_finished());

      return state.edge_stack.top();
    }

    frame metaprogram::to_frame(const edge_descriptor& e_) const
    {
      const auto& vp = get_vertex_property(get_target(e_));
      const auto& ep = get_edge_property(e_);

      switch (get_mode())
      {
      case mode_t::normal:
        return frame(vp.node, vp.source_location, ep.point_of_event, ep.kind);
      default:
      case mode_t::full:
        return frame(vp.node, vp.source_location);
      case mode_t::profile:
        double ratio = [&] {
          if (full_time_taken <= 0.0)
          {
            return 1.0;
          }
          else
          {
            return ep.time_taken / full_time_taken;
          }
        }();
        return frame(vp.node, vp.source_location, ep.point_of_event, ep.kind,
                     ep.time_taken, ratio);
      };
    }

    frame metaprogram::get_current_frame() const
    {
      assert(!is_at_start());
      assert(!is_finished());

      return to_frame(*state.edge_stack.top());
    }

    frame metaprogram::get_root_frame() const
    {
      const auto& vp = get_vertex_property(get_root_vertex());
      return frame(vp.node, vp.source_location);
    }

    backtrace metaprogram::get_backtrace() const
    {
      backtrace tr;

      for (vertex_descriptor current_vertex = get_current_vertex();
           current_vertex != get_root_vertex();)
      {
        assert(state.parent_edge[current_vertex]);

        edge_descriptor parent_edge = *state.parent_edge[current_vertex];
        tr.push_back(to_frame(parent_edge));
        current_vertex = get_source(parent_edge);
      }

      tr.push_back(get_root_frame());

      return tr;
    }

    unsigned metaprogram::get_backtrace_length() const
    {
      assert(!is_finished());

      unsigned length = 0;

      for (vertex_descriptor current_vertex = get_current_vertex();
           current_vertex != get_root_vertex();)
      {
        assert(state.parent_edge[current_vertex]);

        edge_descriptor parent_edge = *state.parent_edge[current_vertex];
        ++length;

        current_vertex = get_source(parent_edge);
      }

      return length;
    }

    unsigned metaprogram::get_traversal_count(vertex_descriptor vertex) const
    {
      if (get_mode() == mode_t::full)
      {
        return get_full_traversal_count(vertex);
      }
      else
      {
        return get_enabled_in_degree(vertex);
      }
    }

    unsigned metaprogram::get_full_traversal_count_helper(
        vertex_descriptor vertex, traversal_counts_t& traversal_counts) const
    {
      if (!traversal_counts[vertex])
      {
        if (vertex == get_root_vertex())
        {
          traversal_counts[vertex] = 1;
        }
        else
        {
          unsigned count = 0;
          for (edge_descriptor edge : get_in_edges(vertex))
          {
            if (get_edge_property(edge).enabled)
            {
              count += get_full_traversal_count_helper(
                  get_source(edge), traversal_counts);
            }
          }
          traversal_counts[vertex] = count;
        }
      }

      assert(traversal_counts[vertex]);
      return *traversal_counts[vertex];
    }

    unsigned
    metaprogram::get_full_traversal_count(vertex_descriptor vertex) const
    {
      if (vertex == get_root_vertex())
      {
        return 0;
      }

      traversal_counts_t traversal_counts(get_num_vertices());

      return get_full_traversal_count_helper(vertex, traversal_counts);
    }

    std::ostream& operator<<(std::ostream& os, metaprogram::mode_t mode)
    {
      switch (mode)
      {
      case metaprogram::mode_t::normal:
        os << "normal";
        break;
      case metaprogram::mode_t::full:
        os << "full";
        break;
      case metaprogram::mode_t::profile:
        os << "profile";
        break;
      }
      return os;
    }
  }
}
