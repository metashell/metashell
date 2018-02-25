#ifndef METASHELL_EVENT_DETAILS_HPP
#define METASHELL_EVENT_DETAILS_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/event_kind.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/type.hpp>
#include <metashell/data/type_or_code_or_error.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    template <event_kind Kind>
    struct event_details
    {
      static_assert(category(Kind) == event_category::template_,
                    "Please create a specialisation for this event kind.");

      type full_name;
      file_location point_of_event;
      file_location source_location;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::template_end>
    {
      double timestamp;
    };

    template <>
    struct event_details<event_kind::macro_expansion>
    {
      cpp_code name;
      boost::optional<std::vector<cpp_code>> args;
      file_location point_of_event;
      file_location source_location;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::macro_expansion_end>
    {
      double timestamp;
    };

    template <>
    struct event_details<event_kind::macro_definition>
    {
      cpp_code name;
      boost::optional<std::vector<cpp_code>> args;
      cpp_code body;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::macro_deletion>
    {
      cpp_code name;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::rescanning>
    {
      cpp_code code;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::expanded_code>
    {
      cpp_code code;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::generated_token>
    {
      token value;
      file_location point_of_event;
      file_location source_location;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::skipped_token>
    {
      token value;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::quote_include>
    {
      boost::filesystem::path path;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::sys_include>
    {
      boost::filesystem::path path;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::include_end>
    {
      double timestamp;
    };

    template <>
    struct event_details<event_kind::preprocessing_condition>
    {
      cpp_code expression;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::preprocessing_condition_result>
    {
      bool result;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::preprocessing_else>
    {
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::preprocessing_endif>
    {
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::error_directive>
    {
      std::string message;
      file_location point_of_event;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::line_directive>
    {
      cpp_code arg;
      file_location point_of_event;
      file_location source_location;
      double timestamp;
    };

    template <>
    struct event_details<event_kind::evaluation_end>
    {
      type_or_code_or_error result;
    };

    template <event_kind Kind>
    constexpr event_kind kind_of(const event_details<Kind>&)
    {
      return Kind;
    }
  }
}

#endif
