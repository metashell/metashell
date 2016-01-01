#ifndef METASHELL_SYSTEM_TEST_PARAGRAPH_HPP
#define METASHELL_SYSTEM_TEST_PARAGRAPH_HPP

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

#include <metashell_system_test/query_json.hpp>
#include <metashell_system_test/json_string.hpp>

#include <rapidjson/document.h>

#include <string>
#include <iosfwd>

namespace metashell_system_test
{
  class paragraph
  {
  public:
    explicit paragraph(const std::string& content_,
                       const std::string& indentation_ = std::string());
    paragraph(const std::string& content_,
              const std::string& rest_of_lines_indentation_,
              const std::string& first_line_indentation_);

    const std::string& content() const;
    const std::string& first_line_indentation() const;
    const std::string& rest_of_lines_indentation() const;

  private:
    std::string _content;
    std::string _first_line_indentation;
    std::string _rest_of_lines_indentation;
  };

  std::ostream& operator<<(std::ostream& out_, const paragraph& p_);

  json_string to_json_string(const paragraph& p_);

  template <class Writer>
  void write(const paragraph& p_, Writer& w_)
  {
    w_.StartObject();

    w_.Key("first_line_indentation");
    w_.String(p_.first_line_indentation().c_str());

    w_.Key("rest_of_lines_indentation");
    w_.String(p_.rest_of_lines_indentation().c_str());

    w_.Key("content");
    w_.String(p_.content().c_str());

    w_.EndObject();
  }

  template <class Encoding, class Allocator>
  bool operator==(const paragraph& p_,
                  const rapidjson::GenericValue<Encoding, Allocator>& v_)
  {
    return members_are({"first_line_indentation", "rest_of_lines_indentation",
                        "content"},
                       v_) &&
           is_string(
               p_.first_line_indentation(), v_["first_line_indentation"]) &&
           is_string(p_.rest_of_lines_indentation(),
                     v_["rest_of_lines_indentation"]) &&
           is_string(p_.content(), v_["content"]);
  }

  template <class Encoding, class Allocator>
  bool operator!=(const paragraph& p_,
                  const rapidjson::GenericValue<Encoding, Allocator>& v_)
  {
    return !metashell_system_test::operator==(p_, v_);
  }
}

#endif
