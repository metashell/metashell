#ifndef METASHELL_SYSTEM_TEST_FRAME_HPP
#define METASHELL_SYSTEM_TEST_FRAME_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/event_kind.hpp>
#include <metashell/system_test/file_location.hpp>
#include <metashell/system_test/json_string.hpp>
#include <metashell/system_test/type.hpp>

#include <pattern/placeholder.hpp>
#include <pattern/to_optional.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <stdexcept>

namespace metashell
{
  namespace system_test
  {
    class frame : boost::equality_comparable<frame, json_string>,
                  boost::equality_comparable<frame>
    {
    public:
      explicit frame(const type& name_);

      template <class Name,
                class SourceLocation,
                class PointOfEvent,
                class Kind>
      frame(Name name_,
            SourceLocation source_location_,
            PointOfEvent point_of_event_,
            Kind kind_)
        : _name(pattern::to_optional(name_)),
          _source_location(pattern::to_optional(source_location_)),
          _point_of_event(pattern::to_optional(point_of_event_)),
          _kind(pattern::to_optional(kind_))
      {
      }

      explicit frame(const json_string& s_);

      template <class JsonDocument>
      frame(const JsonDocument& d_, bool check_type_)
      {
        init(d_, check_type_);
      }

      bool has_name() const;
      const type& name() const;

      bool has_source_location() const;
      const file_location& source_location() const;

      bool has_point_of_event() const;
      const file_location& point_of_event() const;

      bool has_kind() const;

      // precondition: has_kind()
      event_kind kind() const;

      bool operator==(const frame& f_) const;

    private:
      std::optional<type> _name;
      std::optional<file_location> _source_location;
      std::optional<file_location> _point_of_event;
      std::optional<event_kind> _kind;

      template <class JsonDocument>
      void init(const JsonDocument& d_, bool check_type_)
      {
        if (d_.IsObject() && (!check_type_ || (d_.HasMember("type") &&
                                               is_string("frame", d_["type"]))))
        {
          if (d_.HasMember("name"))
          {
            if (d_["name"].IsString())
            {
              const auto& name = d_["name"];
              _name =
                  type(std::string(name.GetString(), name.GetStringLength()));
            }
            else
            {
              throw std::runtime_error("name field should be string");
            }
          }
          if (d_.HasMember("kind"))
          {
            _kind = parse_kind(d_["kind"]);
          }
          if (d_.HasMember("source_location"))
          {
            _source_location = file_location(d_["source_location"]);
          }
          if (d_.HasMember("point_of_event"))
          {
            _point_of_event = file_location(d_["point_of_event"]);
          }
        }
        else
        {
          throw std::runtime_error("Invalid frame");
        }
      }
    };

    std::ostream& operator<<(std::ostream& o_, const frame& f_);

    json_string to_json_string(const frame& f_);

    bool operator==(const frame& frame_, const json_string& s_);

    template <class JsonDocument>
    bool matches(const frame& frame_, const JsonDocument& doc_)
    {
      return no_other_members_than(
                 {"type", "name", "source_location", "kind", "point_of_event",
                  "time_taken", "time_taken_ratio"},
                 doc_) &&
             has_members({"name", "source_location"}, doc_) &&
             matches(frame_.name(), doc_["name"]) &&
             (!frame_.has_kind() ||
              (doc_.HasMember("kind") &&
               is_string(to_string(frame_.kind()), doc_["kind"])));
    }
  }
}

#endif
