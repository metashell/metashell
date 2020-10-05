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

#include <metashell/system_test/json_string.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <iostream>
#include <regex>

namespace metashell
{
  namespace system_test
  {
    json_string::json_string(const std::string& json_) : _json(json_) {}

    const std::string& json_string::get() const { return _json; }

    json_string& json_string::operator+=(const json_string& s_)
    {
      _json += s_._json;
      return *this;
    }

    json_string::size_type json_string::size() const { return _json.size(); }

    json_string::size_type json_string::find(const json_string& s_) const
    {
      return _json.find(s_._json);
    }

    json_string json_string::substr(json_string::size_type pos_,
                                    json_string::size_type len_) const
    {
      return json_string(_json.substr(pos_, len_));
    }

    bool operator==(const json_string& a_, const json_string& b_)
    {
      // Not testing point_of_event and source_location
      const std::regex filter_poi(R"(,"point_of_event":"[^"]*")");
      const std::regex filter_sl(R"(,"source_location":"[^"]*")");

      const auto filtered_a = std::regex_replace(a_.get(), filter_poi, "");
      const auto filtered_b = std::regex_replace(b_.get(), filter_poi, "");

      return std::regex_replace(filtered_a, filter_sl, "") ==
             std::regex_replace(filtered_b, filter_sl, "");
    }

    std::ostream& operator<<(std::ostream& out_, const json_string& s_)
    {
      return out_ << s_.get();
    }

    json_string array(std::initializer_list<json_string> items_)
    {
      return json_string(
          "[" +
          boost::algorithm::join(
              items_ | boost::adaptors::transformed(
                           [](const json_string& s_) { return s_.get(); }),
              ",") +
          "]");
    }
  } // namespace system_test
} // namespace metashell
