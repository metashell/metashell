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

#include <metashell/system_test/instantiation_kind.hpp>
#include <metashell/system_test/json_string.hpp>
#include <metashell/system_test/type.hpp>

#include <pattern/placeholder.hpp>

#include <boost/operators.hpp>
#include <boost/optional.hpp>

#include <iosfwd>

namespace metashell
{
  namespace system_test
  {
    class frame : boost::equality_comparable<frame, json_string>
    {
    public:
      explicit frame(const type& name_);

      frame(const type& name_,
            pattern::placeholder,
            pattern::placeholder,
            instantiation_kind kind_);

      const type& name() const;

      bool has_kind() const;

      // precondition: has_kind()
      instantiation_kind kind() const;

    private:
      type _name;
      boost::optional<instantiation_kind> _kind;
    };

    std::ostream& operator<<(std::ostream& o_, const frame& f_);

    json_string to_json_string(const frame& f_);

    bool operator==(const frame& frame_, const json_string& s_);

    template <class JsonDocument>
    bool matches(const frame& frame_, const JsonDocument& doc_)
    {
      return no_other_members_than(
                 {"type", "name", "source_location", "kind",
                  "point_of_instantiation", "time_taken", "time_taken_ratio"},
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
