#ifndef METASHELL_METAPROGRAM_BUILDER_HPP
#define METASHELL_METAPROGRAM_BUILDER_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/include_argument.hpp>
#include <metashell/data/metaprogram.hpp>

#include <stack>
#include <type_traits>

namespace metashell
{
  class metaprogram_builder
  {
  public:
    template <class Container>
    metaprogram_builder(Container&& trace,
                        data::metaprogram::mode_t mode,
                        const data::cpp_code& root_name)
      : metaprogram_builder(mode, root_name)
    {
      for (const data::event_data& event : trace)
      {
        handle_event(event);
      }
    }

    const data::metaprogram& get_metaprogram() const;

  private:
    typedef data::metaprogram::vertex_descriptor vertex_descriptor;
    typedef data::metaprogram::edge_descriptor edge_descriptor;

    typedef std::tuple<data::metaprogram_node, data::file_location>
        element_key_t;
    typedef std::map<element_key_t, vertex_descriptor> element_vertex_map_t;

    metaprogram_builder(data::metaprogram::mode_t mode,
                        const data::cpp_code& root_name);

    void handle_event(const data::event_data& details);

    vertex_descriptor add_vertex(const data::metaprogram_node& node,
                                 const data::file_location& source_location);

    vertex_descriptor add_vertex(const data::timeless_event_details<
                                 data::event_kind::memoization>& event)
    {
      return add_vertex(name(event), source_location(event));
    }

    vertex_descriptor
    add_vertex(const data::timeless_event_details<
               data::event_kind::template_instantiation>& event)
    {
      return add_vertex(name(event), source_location(event));
    }

    template <data::event_kind Kind>
    vertex_descriptor
    add_vertex(const data::timeless_event_details<Kind>& event)
    {
      return add_vertex(unique_value(name(event)), source_location(event));
    }

    void pop_edge(data::event_kind end_kind, double timestamp);

    template <data::event_kind Kind>
    edge_descriptor add_edge(const data::event_details<Kind>& details)
    {
      return mp.add_edge(edge_stack.empty() ? mp.get_root_vertex() :
                                              mp.get_target(edge_stack.top()),
                         add_vertex(details.what), Kind,
                         point_of_event(details.what), details.timestamp);
    }

    template <data::event_kind Kind>
    typename std::enable_if<data::relative_depth_of(Kind) ==
                            data::relative_depth::open>::type
    handle_event_impl(const data::event_details<Kind>& details)
    {
      edge_stack.push(add_edge(details));
    }

    template <data::event_kind Kind>
    typename std::enable_if<data::relative_depth_of(Kind) ==
                            data::relative_depth::flat>::type
    handle_event_impl(const data::event_details<Kind>& details)
    {
      add_edge(details);
    }

    template <data::event_kind Kind>
    typename std::enable_if<data::relative_depth_of(Kind) ==
                            data::relative_depth::close>::type
    handle_event_impl(const data::event_details<Kind>& details)
    {
      pop_edge(kind_of(details.what), details.timestamp);
    }

    template <data::event_kind Kind>
    typename std::enable_if<data::relative_depth_of(Kind) ==
                            data::relative_depth::end>::type
    handle_event_impl(const data::event_details<Kind>& details)
    {
      mp.set_evaluation_result(details.what.result);
    }

    data::metaprogram mp;

    std::stack<edge_descriptor> edge_stack;

    element_vertex_map_t element_vertex_map;
  };
}

#endif
