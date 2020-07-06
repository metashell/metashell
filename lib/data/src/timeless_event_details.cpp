// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/timeless_event_details.hpp>

namespace metashell
{
  namespace data
  {
    std::optional<type>
    type_of(const timeless_event_details<event_kind::template_end>&)
    {
      return std::nullopt;
    }

    cpp_code
    name(const timeless_event_details<event_kind::macro_expansion>& details_)
    {
      return details_.args ?
                 details_.name + cpp_code("(") +
                     join(*details_.args, cpp_code(",")) + cpp_code(")") :
                 details_.name;
    }

    file_location source_location(
        const timeless_event_details<event_kind::macro_expansion>& details_)
    {
      return details_.source_location;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::macro_expansion>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::macro_definition>& details_)
    {
      return details_.name +
             (details_.args ?
                  cpp_code("(") + join(*details_.args, cpp_code(", ")) +
                      cpp_code(")") :
                  cpp_code()) +
             cpp_code(" ") + details_.body;
    }

    file_location source_location(
        const timeless_event_details<event_kind::macro_definition>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::macro_definition>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::macro_deletion>& details_)
    {
      return details_.name;
    }

    file_location source_location(
        const timeless_event_details<event_kind::macro_deletion>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::macro_deletion>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::rescanning>& details_)
    {
      return details_.code;
    }

    file_location source_location(
        const timeless_event_details<event_kind::rescanning>& details_)
    {
      return details_.source_location;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::rescanning>& details_)
    {
      return source_location(details_);
    }

    cpp_code
    name(const timeless_event_details<event_kind::expanded_code>& details_)
    {
      return details_.code;
    }

    file_location source_location(
        const timeless_event_details<event_kind::expanded_code>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::expanded_code>& details_)
    {
      return details_.point_of_event;
    }

    token
    name(const timeless_event_details<event_kind::generated_token>& details_)
    {
      return details_.value;
    }

    file_location source_location(
        const timeless_event_details<event_kind::generated_token>& details_)
    {
      return details_.source_location;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::generated_token>& details_)
    {
      return details_.point_of_event;
    }

    token
    name(const timeless_event_details<event_kind::skipped_token>& details_)
    {
      return details_.value;
    }

    file_location source_location(
        const timeless_event_details<event_kind::skipped_token>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::skipped_token>& details_)
    {
      return details_.point_of_event;
    }

    boost::filesystem::path
    name(const timeless_event_details<event_kind::quote_include>& details_)
    {
      return details_.path;
    }

    file_location source_location(
        const timeless_event_details<event_kind::quote_include>& details_)
    {
      return file_location(details_.path, 1, 1);
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::quote_include>& details_)
    {
      return details_.point_of_event;
    }

    boost::filesystem::path
    name(const timeless_event_details<event_kind::sys_include>& details_)
    {
      return details_.path;
    }

    file_location source_location(
        const timeless_event_details<event_kind::sys_include>& details_)
    {
      return file_location(details_.path, 1, 1);
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::sys_include>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::preprocessing_condition>&
             details_)
    {
      return details_.expression;
    }

    file_location source_location(
        const timeless_event_details<event_kind::preprocessing_condition>&
            details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(const timeless_event_details<
                                 event_kind::preprocessing_condition>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code name(const timeless_event_details<
                  event_kind::preprocessing_condition_result>& details_)
    {
      return cpp_code(details_.result ? "true" : "false");
    }

    file_location
    source_location(const timeless_event_details<
                    event_kind::preprocessing_condition_result>& details_)
    {
      return details_.source_location;
    }

    file_location
    point_of_event(const timeless_event_details<
                   event_kind::preprocessing_condition_result>& details_)
    {
      return source_location(details_);
    }

    cpp_code name(const timeless_event_details<event_kind::preprocessing_else>&)
    {
      return cpp_code("#else");
    }

    file_location source_location(
        const timeless_event_details<event_kind::preprocessing_else>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::preprocessing_else>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::preprocessing_endif>&)
    {
      return cpp_code("#endif");
    }

    file_location source_location(
        const timeless_event_details<event_kind::preprocessing_endif>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::preprocessing_endif>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::error_directive>& details_)
    {
      return cpp_code("#error " + details_.message);
    }

    file_location source_location(
        const timeless_event_details<event_kind::error_directive>& details_)
    {
      return details_.point_of_event;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::error_directive>& details_)
    {
      return details_.point_of_event;
    }

    cpp_code
    name(const timeless_event_details<event_kind::line_directive>& details_)
    {
      return cpp_code("#line ") + details_.arg;
    }

    file_location source_location(
        const timeless_event_details<event_kind::line_directive>& details_)
    {
      return details_.source_location;
    }

    file_location point_of_event(
        const timeless_event_details<event_kind::line_directive>& details_)
    {
      return details_.point_of_event;
    }

    type_or_code_or_error result_of(
        const timeless_event_details<event_kind::evaluation_end>& details_)
    {
      return details_.result;
    }
  }
}
