#ifndef METASHELL_SYSTEM_TEST_TYPE_HPP
#define METASHELL_SYSTEM_TEST_TYPE_HPP

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

#include <metashell_system_test/json_string.hpp>
#include <metashell_system_test/placeholder.hpp>
#include <metashell_system_test/query_json.hpp>

#include <boost/optional.hpp>

#include <string>
#include <iosfwd>

namespace metashell_system_test
{
  class type
  {
  public:
    explicit type(const std::string& name_);
    explicit type(placeholder);
  
    bool name_specified() const;
    const std::string& name() const;
  private:
    boost::optional<std::string> _name;
  };

  std::ostream& operator<<(std::ostream& out_, const type& type_);

  json_string to_json_string(const type& t_);

  bool operator==(const type& type_, const json_string& s_);

  template <class JsonDocument>
  bool matches(const type& type_, const JsonDocument& doc_)
  {
    return !type_.name_specified() || is_string(type_.name(), doc_);
  }
}

#endif

