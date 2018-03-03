
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

#include <metashell/metaprogram_builder.hpp>

#include <metashell/exception.hpp>

#include <boost/algorithm/string/join.hpp>

namespace metashell
{

  metaprogram_builder::metaprogram_builder(
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::file_location& root_source_location)
    : mp(mode,
         root_name,
         root_source_location,
         data::type_or_code_or_error(
             "Internal Metashell error: metaprogram not finished yet"))
  {
  }

  void metaprogram_builder::handle_event(const data::event_data& details)
  {
    mpark::visit(
        [this](const auto& det) { this->handle_event_impl(det); }, details);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::macro_expansion>& details)
  {
    data::cpp_code call = details.name;
    if (details.args)
    {
      call += "(" + boost::algorithm::join(*details.args, ",") + ")";
    }

    vertex_descriptor vertex =
        add_vertex(unique_value(call), details.source_location);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    auto edge =
        mp.add_edge(top_vertex, vertex, data::event_kind::macro_expansion,
                    details.point_of_event, details.timestamp);
    edge_stack.push(edge);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::rescanning>& details)
  {
    if (edge_stack.empty())
    {
      throw exception("Mismatched macro expansion begin and rescanning events");
    }
    auto& ep = mp.get_edge_property(edge_stack.top());

    vertex_descriptor vertex =
        add_vertex(unique_value(details.code), ep.point_of_event);
    vertex_descriptor top_vertex = mp.get_target(edge_stack.top());

    auto edge = mp.add_edge(top_vertex, vertex, data::event_kind::rescanning,
                            ep.point_of_event, details.timestamp);
    edge_stack.push(edge);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::expanded_code>& details)
  {
    vertex_descriptor vertex =
        add_vertex(unique_value(details.code), details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::expanded_code,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::macro_expansion_end>& details)
  {
    // one rescanning and one macro expansion
    for (int i = 0; i != 2; ++i)
    {
      if (edge_stack.empty())
      {
        throw exception("Mismatched macro expansion begin and end events");
      }
      auto& ep = mp.get_edge_property(edge_stack.top());
      ep.time_taken = details.timestamp - ep.begin_timestamp;

      edge_stack.pop();
    }
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::quote_include>& details)
  {
    vertex_descriptor vertex = add_vertex(
        unique_value(details.path), data::file_location(details.path, 1, 1));
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    auto edge = mp.add_edge(top_vertex, vertex, data::event_kind::quote_include,
                            details.point_of_event, details.timestamp);
    edge_stack.push(edge);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::sys_include>& details)
  {
    vertex_descriptor vertex = add_vertex(
        unique_value(details.path), data::file_location(details.path, 1, 1));
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    auto edge = mp.add_edge(top_vertex, vertex, data::event_kind::sys_include,
                            details.point_of_event, details.timestamp);
    edge_stack.push(edge);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::include_end>& details)
  {
    if (edge_stack.empty())
    {
      throw exception("Mismatched IncludeBegin and IncludeEnd events");
    }
    auto& ep = mp.get_edge_property(edge_stack.top());
    ep.time_taken = details.timestamp - ep.begin_timestamp;

    edge_stack.pop();
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::macro_definition>& details)
  {
    vertex_descriptor vertex = add_vertex(
        unique_value(
            details.name +
            (details.args ?
                 "(" + boost::algorithm::join(*details.args, ", ") + ")" :
                 data::cpp_code()) +
            " " + details.body),
        details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::macro_definition,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::macro_deletion>& details)
  {
    vertex_descriptor vertex =
        add_vertex(unique_value(details.name), details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::macro_deletion,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::preprocessing_condition>&
          details)
  {
    vertex_descriptor vertex =
        add_vertex(unique_value(details.expression), details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    auto edge = mp.add_edge(top_vertex, vertex,
                            data::event_kind::preprocessing_condition,
                            details.point_of_event, details.timestamp);
    edge_stack.push(edge);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<
          data::event_kind::preprocessing_condition_result>& details)
  {
    if (edge_stack.empty())
    {
      throw exception(
          "Mismatched PreprocessingConditionBegin and "
          "PreprocessingConditionEnd events");
    }
    auto& ep = mp.get_edge_property(edge_stack.top());
    ep.time_taken = details.timestamp - ep.begin_timestamp;

    vertex_descriptor vertex = add_vertex(
        unique_value(data::cpp_code(details.result ? "true" : "false")),
        ep.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex,
                data::event_kind::preprocessing_condition_result,
                ep.point_of_event, details.timestamp);

    edge_stack.pop();
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::preprocessing_else>& details)
  {
    vertex_descriptor vertex = add_vertex(
        unique_value(data::cpp_code("#else")), details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::preprocessing_else,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::preprocessing_endif>& details)
  {
    vertex_descriptor vertex = add_vertex(
        unique_value(data::cpp_code("#endif")), details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::preprocessing_endif,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::error_directive>& details)
  {
    vertex_descriptor vertex =
        add_vertex(unique_value(data::cpp_code("#error " + details.message)),
                   details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::error_directive,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::line_directive>& details)
  {
    vertex_descriptor vertex = add_vertex(
        unique_value("#line " + details.arg), details.source_location);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::line_directive,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_template_begin(
      data::event_kind kind,
      const data::type& type,
      const data::file_location& point_of_event,
      const data::file_location& source_location,
      double timestamp)
  {
    vertex_descriptor vertex = add_vertex(type, source_location);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    auto edge =
        mp.add_edge(top_vertex, vertex, kind, point_of_event, timestamp);
    edge_stack.push(edge);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::template_end>& details)
  {
    if (edge_stack.empty())
    {
      throw exception(
          "Mismatched Templight TemplateBegin and TemplateEnd events");
    }
    auto& ep = mp.get_edge_property(edge_stack.top());
    ep.time_taken = details.timestamp - ep.begin_timestamp;

    edge_stack.pop();
  }

  const data::metaprogram& metaprogram_builder::get_metaprogram() const
  {
    if (!edge_stack.empty())
    {
      throw exception("Some Templight TemplateEnd events are missing");
    }
    return mp;
  }

  metaprogram_builder::vertex_descriptor
  metaprogram_builder::add_vertex(const data::metaprogram_node& node,
                                  const data::file_location& source_location)
  {
    element_vertex_map_t::iterator pos;
    bool inserted;

    std::tie(pos, inserted) = element_vertex_map.insert(std::make_pair(
        std::make_tuple(node, source_location), vertex_descriptor()));

    if (inserted)
    {
      pos->second = mp.add_vertex(node, source_location);
    }
    return pos->second;
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::evaluation_end>& details)
  {
    mp.set_evaluation_result(details.result);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::skipped_token>& details)
  {
    vertex_descriptor vertex =
        add_vertex(unique_value(details.value), details.point_of_event);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::skipped_token,
                details.point_of_event, details.timestamp);
  }

  void metaprogram_builder::handle_event_impl(
      const data::event_details<data::event_kind::generated_token>& details)
  {
    vertex_descriptor vertex =
        add_vertex(unique_value(details.value), details.source_location);
    vertex_descriptor top_vertex = edge_stack.empty() ?
                                       mp.get_root_vertex() :
                                       mp.get_target(edge_stack.top());

    mp.add_edge(top_vertex, vertex, data::event_kind::generated_token,
                details.point_of_event, details.timestamp);
  }
}
