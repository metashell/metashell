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
#include <metashell/data/event_details.hpp>
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
    metaprogram_builder(data::metaprogram::mode_t mode,
                        const data::cpp_code& root_name,
                        const data::file_location& root_source_location);

    template <data::event_kind Kind>
    typename std::enable_if<category(Kind) ==
                            data::event_category::template_>::type
    handle_event(const data::event_details<Kind>& details)
    {
      handle_template_begin(Kind, details.full_name, details.point_of_event,
                            details.source_location, details.timestamp);
    }

    void handle_event(
        const data::event_details<data::event_kind::template_end>& details);

    void handle_event(
        const data::event_details<data::event_kind::macro_expansion>& details);

    void handle_event(
        const data::event_details<data::event_kind::rescanning>& details);

    void handle_event(
        const data::event_details<data::event_kind::expanded_code>& details);

    void handle_event(const data::event_details<
                      data::event_kind::macro_expansion_end>& details);

    void handle_event(
        const data::event_details<data::event_kind::generated_token>& details);

    void handle_event(
        const data::event_details<data::event_kind::skipped_token>& details);

    void handle_event(
        const data::event_details<data::event_kind::quote_include>& details);

    void handle_event(
        const data::event_details<data::event_kind::sys_include>& details);

    void handle_event(
        const data::event_details<data::event_kind::include_end>& details);

    void handle_event(
        const data::event_details<data::event_kind::macro_definition>& details);

    void handle_event(
        const data::event_details<data::event_kind::macro_deletion>& details);

    void handle_event(const data::event_details<
                      data::event_kind::preprocessing_condition>& details);

    void
    handle_event(const data::event_details<
                 data::event_kind::preprocessing_condition_result>& details);

    void handle_event(const data::event_details<
                      data::event_kind::preprocessing_else>& details);

    void handle_event(const data::event_details<
                      data::event_kind::preprocessing_endif>& details);

    void handle_event(
        const data::event_details<data::event_kind::error_directive>& details);

    void handle_event(
        const data::event_details<data::event_kind::line_directive>& details);

    void handle_event(
        const data::event_details<data::event_kind::evaluation_end>& details);

    const data::metaprogram& get_metaprogram() const;

  private:
    typedef data::metaprogram::vertex_descriptor vertex_descriptor;
    typedef data::metaprogram::edge_descriptor edge_descriptor;

    typedef std::tuple<data::metaprogram_node, data::file_location>
        element_key_t;
    typedef std::map<element_key_t, vertex_descriptor> element_vertex_map_t;

    vertex_descriptor add_vertex(const data::metaprogram_node& node,
                                 const data::file_location& source_location);

    void handle_template_begin(data::event_kind kind,
                               const data::type& type,
                               const data::file_location& point_of_event,
                               const data::file_location& source_location,
                               double timestamp);

    data::metaprogram mp;

    std::stack<edge_descriptor> edge_stack;

    element_vertex_map_t element_vertex_map;
  };
}

#endif
