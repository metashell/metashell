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
#include <type_traits>
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

    template <event_kind Kind>
    typename std::enable_if<category(Kind) == event_category::template_ &&
                                Kind != event_kind::template_end,
                            type>::type
    name(const event_details<Kind>& details_)
    {
      return details_.full_name;
    }

    template <event_kind Kind>
    typename std::enable_if<category(Kind) == event_category::template_ &&
                                Kind != event_kind::template_end,
                            file_location>::type
    source_location(const event_details<Kind>& details_)
    {
      return details_.source_location;
    }

    template <event_kind Kind>
    typename std::enable_if<category(Kind) == event_category::template_ &&
                                Kind != event_kind::template_end,
                            file_location>::type
    point_of_event(const event_details<Kind>& details_)
    {
      return details_.point_of_event;
    }

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

    cpp_code name(const event_details<event_kind::macro_expansion>& details_);

    file_location
    source_location(const event_details<event_kind::macro_expansion>& details_);

    file_location
    point_of_event(const event_details<event_kind::macro_expansion>& details_);

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

    cpp_code name(const event_details<event_kind::macro_definition>& details_);

    file_location source_location(
        const event_details<event_kind::macro_definition>& details_);

    file_location
    point_of_event(const event_details<event_kind::macro_definition>& details_);

    template <>
    struct event_details<event_kind::macro_deletion>
    {
      cpp_code name;
      file_location point_of_event;
      double timestamp;
    };

    cpp_code name(const event_details<event_kind::macro_deletion>& details_);

    file_location
    source_location(const event_details<event_kind::macro_deletion>& details_);

    file_location
    point_of_event(const event_details<event_kind::macro_deletion>& details_);

    template <>
    struct event_details<event_kind::rescanning>
    {
      cpp_code code;
      file_location source_location;
      double timestamp;
    };

    cpp_code name(const event_details<event_kind::rescanning>& details_);

    file_location
    source_location(const event_details<event_kind::rescanning>& details_);

    file_location
    point_of_event(const event_details<event_kind::rescanning>& details_);

    template <>
    struct event_details<event_kind::rescanning_end>
    {
      double timestamp;
    };

    template <>
    struct event_details<event_kind::expanded_code>
    {
      cpp_code code;
      file_location point_of_event;
      double timestamp;
    };

    cpp_code name(const event_details<event_kind::expanded_code>& details_);

    file_location
    source_location(const event_details<event_kind::expanded_code>& details_);

    file_location
    point_of_event(const event_details<event_kind::expanded_code>& details_);

    template <>
    struct event_details<event_kind::generated_token>
    {
      token value;
      file_location point_of_event;
      file_location source_location;
      double timestamp;
    };

    token name(const event_details<event_kind::generated_token>& details_);

    file_location
    source_location(const event_details<event_kind::generated_token>& details_);

    file_location
    point_of_event(const event_details<event_kind::generated_token>& details_);

    template <>
    struct event_details<event_kind::skipped_token>
    {
      token value;
      file_location point_of_event;
      double timestamp;
    };

    token name(const event_details<event_kind::skipped_token>& details_);

    file_location
    source_location(const event_details<event_kind::skipped_token>& details_);

    file_location
    point_of_event(const event_details<event_kind::skipped_token>& details_);

    template <>
    struct event_details<event_kind::quote_include>
    {
      boost::filesystem::path path;
      file_location point_of_event;
      double timestamp;
    };

    boost::filesystem::path
    name(const event_details<event_kind::quote_include>& details_);

    file_location
    source_location(const event_details<event_kind::quote_include>& details_);

    file_location
    point_of_event(const event_details<event_kind::quote_include>& details_);

    template <>
    struct event_details<event_kind::sys_include>
    {
      boost::filesystem::path path;
      file_location point_of_event;
      double timestamp;
    };

    boost::filesystem::path
    name(const event_details<event_kind::sys_include>& details_);

    file_location
    source_location(const event_details<event_kind::sys_include>& details_);

    file_location
    point_of_event(const event_details<event_kind::sys_include>& details_);

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

    cpp_code
    name(const event_details<event_kind::preprocessing_condition>& details_);

    file_location source_location(
        const event_details<event_kind::preprocessing_condition>& details_);

    file_location point_of_event(
        const event_details<event_kind::preprocessing_condition>& details_);

    template <>
    struct event_details<event_kind::preprocessing_condition_result>
    {
      bool result;
      file_location source_location;
      double timestamp;
    };

    cpp_code
    name(const event_details<event_kind::preprocessing_condition_result>&
             details_);

    file_location source_location(
        const event_details<event_kind::preprocessing_condition_result>&
            details_);

    file_location point_of_event(
        const event_details<event_kind::preprocessing_condition_result>&
            details_);

    template <>
    struct event_details<event_kind::preprocessing_condition_end>
    {
      double timestamp;
    };

    template <>
    struct event_details<event_kind::preprocessing_else>
    {
      file_location point_of_event;
      double timestamp;
    };

    cpp_code
    name(const event_details<event_kind::preprocessing_else>& details_);

    file_location source_location(
        const event_details<event_kind::preprocessing_else>& details_);

    file_location point_of_event(
        const event_details<event_kind::preprocessing_else>& details_);

    template <>
    struct event_details<event_kind::preprocessing_endif>
    {
      file_location point_of_event;
      double timestamp;
    };

    cpp_code
    name(const event_details<event_kind::preprocessing_endif>& details_);

    file_location source_location(
        const event_details<event_kind::preprocessing_endif>& details_);

    file_location point_of_event(
        const event_details<event_kind::preprocessing_endif>& details_);

    template <>
    struct event_details<event_kind::error_directive>
    {
      std::string message;
      file_location point_of_event;
      double timestamp;
    };

    cpp_code name(const event_details<event_kind::error_directive>& details_);

    file_location
    source_location(const event_details<event_kind::error_directive>& details_);

    file_location
    point_of_event(const event_details<event_kind::error_directive>& details_);

    template <>
    struct event_details<event_kind::line_directive>
    {
      cpp_code arg;
      file_location point_of_event;
      file_location source_location;
      double timestamp;
    };

    cpp_code name(const event_details<event_kind::line_directive>& details_);

    file_location
    source_location(const event_details<event_kind::line_directive>& details_);

    file_location
    point_of_event(const event_details<event_kind::line_directive>& details_);

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
