#ifndef METASHELL_DATA_TIMELESS_EVENT_DETAILS_HPP
#define METASHELL_DATA_TIMELESS_EVENT_DETAILS_HPP

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
#include <metashell/data/fields.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/type.hpp>
#include <metashell/data/type_or_code_or_error.hpp>

#include <boost/filesystem/path.hpp>

#include <optional>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    template <event_kind Kind>
    struct timeless_event_details
    {
      static_assert(category(Kind) == event_category::template_,
                    "Please create a specialisation for this event kind.");

      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((type, full_name))
        ((file_location, point_of_event))
        ((file_location, source_location))
      );

      // clang-format on
    };

    template <event_kind Kind>
    typename std::enable_if<!non_end_template_event(Kind),
                            std::optional<type>>::type
    type_of(const timeless_event_details<Kind>&)
    {
      return std::nullopt;
    }

    template <event_kind Kind>
    typename std::enable_if<!non_end_template_event(Kind)>::type
    set_type(timeless_event_details<Kind>&, const type&)
    {
    }

    template <event_kind Kind>
    typename std::enable_if<non_end_template_event(Kind), bool>::type
    is_remove_ptr(const timeless_event_details<Kind>& details_)
    {
      return is_remove_ptr(details_.full_name);
    }

    template <event_kind Kind>
    typename std::enable_if<!non_end_template_event(Kind), bool>::type
    is_remove_ptr(const timeless_event_details<Kind>&)
    {
      return false;
    }

    template <event_kind Kind>
    typename std::enable_if<non_end_template_event(Kind),
                            std::optional<type>>::type
    type_of(const timeless_event_details<Kind>& details_)
    {
      return details_.full_name;
    }

    template <event_kind Kind>
    typename std::enable_if<non_end_template_event(Kind)>::type
    set_type(timeless_event_details<Kind>& details_, type type_)
    {
      details_.full_name = std::move(type_);
    }

    template <event_kind Kind>
    typename std::enable_if<non_end_template_event(Kind), type>::type
    name(const timeless_event_details<Kind>& details_)
    {
      return details_.full_name;
    }

    template <event_kind Kind>
    typename std::enable_if<non_end_template_event(Kind), file_location>::type
    source_location(const timeless_event_details<Kind>& details_)
    {
      return details_.source_location;
    }

    template <event_kind Kind>
    typename std::enable_if<non_end_template_event(Kind), file_location>::type
    point_of_event(const timeless_event_details<Kind>& details_)
    {
      return details_.point_of_event;
    }

    template <>
    struct timeless_event_details<event_kind::template_end>
    {
      METASHELL_DATA_NO_FIELDS(timeless_event_details)
    };

    template <>
    struct timeless_event_details<event_kind::macro_expansion>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, name))
        ((std::optional<std::vector<cpp_code>>, args))
        ((file_location, point_of_event))
        ((file_location, source_location))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::macro_expansion>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::macro_expansion>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::macro_expansion>& details_);

    template <>
    struct timeless_event_details<event_kind::macro_expansion_end>
    {
      METASHELL_DATA_NO_FIELDS(timeless_event_details)
    };

    template <>
    struct timeless_event_details<event_kind::macro_definition>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, name))
        ((std::optional<std::vector<cpp_code>>, args))
        ((cpp_code, body))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::macro_definition>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::macro_definition>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::macro_definition>& details_);

    template <>
    struct timeless_event_details<event_kind::macro_deletion>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, name))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::macro_deletion>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::macro_deletion>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::macro_deletion>& details_);

    template <>
    struct timeless_event_details<event_kind::rescanning>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, code))
        ((file_location, source_location))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::rescanning>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::rescanning>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::rescanning>& details_);

    template <>
    struct timeless_event_details<event_kind::rescanning_end>
    {
      METASHELL_DATA_NO_FIELDS(timeless_event_details)
    };

    template <>
    struct timeless_event_details<event_kind::expanded_code>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, code))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::expanded_code>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::expanded_code>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::expanded_code>& details_);

    template <>
    struct timeless_event_details<event_kind::generated_token>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((token, value))
        ((file_location, point_of_event))
        ((file_location, source_location))
      )

      // clang-format on
    };

    token
    name(const timeless_event_details<event_kind::generated_token>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::generated_token>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::generated_token>& details_);

    template <>
    struct timeless_event_details<event_kind::skipped_token>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((token, value))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    token
    name(const timeless_event_details<event_kind::skipped_token>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::skipped_token>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::skipped_token>& details_);

    template <>
    struct timeless_event_details<event_kind::quote_include>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((boost::filesystem::path, path))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    boost::filesystem::path
    name(const timeless_event_details<event_kind::quote_include>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::quote_include>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::quote_include>& details_);

    template <>
    struct timeless_event_details<event_kind::sys_include>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((boost::filesystem::path, path))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    boost::filesystem::path
    name(const timeless_event_details<event_kind::sys_include>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::sys_include>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::sys_include>& details_);

    template <>
    struct timeless_event_details<event_kind::include_end>
    {
      METASHELL_DATA_NO_FIELDS(timeless_event_details)
    };

    template <>
    struct timeless_event_details<event_kind::preprocessing_condition>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, expression))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::preprocessing_condition>&
             details_);

    file_location source_location(
        const timeless_event_details<event_kind::preprocessing_condition>&
            details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::preprocessing_condition>&
            details_);

    template <>
    struct timeless_event_details<event_kind::preprocessing_condition_result>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((bool, result))
        ((file_location, source_location))
      )

      // clang-format on
    };

    cpp_code name(const timeless_event_details<
                  event_kind::preprocessing_condition_result>& details_);

    file_location
    source_location(const timeless_event_details<
                    event_kind::preprocessing_condition_result>& details_);

    file_location
    point_of_event(const timeless_event_details<
                   event_kind::preprocessing_condition_result>& details_);

    template <>
    struct timeless_event_details<event_kind::preprocessing_condition_end>
    {
      METASHELL_DATA_NO_FIELDS(timeless_event_details)
    };

    template <>
    struct timeless_event_details<event_kind::preprocessing_else>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code name(
        const timeless_event_details<event_kind::preprocessing_else>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::preprocessing_else>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::preprocessing_else>& details_);

    template <>
    struct timeless_event_details<event_kind::preprocessing_endif>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code name(const timeless_event_details<event_kind::preprocessing_endif>&
                      details_);

    file_location source_location(const timeless_event_details<
                                  event_kind::preprocessing_endif>& details_);

    file_location point_of_event(const timeless_event_details<
                                 event_kind::preprocessing_endif>& details_);

    template <>
    struct timeless_event_details<event_kind::error_directive>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((std::string, message))
        ((file_location, point_of_event))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::error_directive>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::error_directive>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::error_directive>& details_);

    template <>
    struct timeless_event_details<event_kind::line_directive>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((cpp_code, arg))
        ((file_location, point_of_event))
        ((file_location, source_location))
      )

      // clang-format on
    };

    cpp_code
    name(const timeless_event_details<event_kind::line_directive>& details_);

    file_location source_location(
        const timeless_event_details<event_kind::line_directive>& details_);

    file_location point_of_event(
        const timeless_event_details<event_kind::line_directive>& details_);

    template <>
    struct timeless_event_details<event_kind::evaluation_end>
    {
      // clang-format off

      METASHELL_DATA_FIELDS(
        timeless_event_details,

        ((type_or_code_or_error, result))
      )

      // clang-format on
    };

    type_or_code_or_error result_of(
        const timeless_event_details<event_kind::evaluation_end>& details_);

    template <event_kind Kind>
    bool operator==(const timeless_event_details<Kind>& a_,
                    const timeless_event_details<Kind>& b_)
    {
      return a_.to_tuple() == b_.to_tuple();
    }

    template <event_kind Kind1, event_kind Kind2>
    bool operator==(const timeless_event_details<Kind1>&,
                    const timeless_event_details<Kind2>&)
    {
      return false;
    }

    template <event_kind Kind1, event_kind Kind2>
    bool operator!=(const timeless_event_details<Kind1>& a_,
                    const timeless_event_details<Kind2>& b_)
    {
      return !(a_ == b_);
    }

    template <event_kind Kind>
    bool operator<(const timeless_event_details<Kind>& a_,
                   const timeless_event_details<Kind>& b_)
    {
      return a_.to_tuple() < b_.to_tuple();
    }

    template <event_kind Kind>
    constexpr event_kind kind_of(const timeless_event_details<Kind>&)
    {
      return Kind;
    }
  }
}

#endif
