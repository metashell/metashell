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

#include <metashell_system_test/paragraph.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>

using namespace metashell_system_test;

paragraph::paragraph(const std::string& content_,
                     const std::string& indentation_)
  : _content(content_),
    _first_line_indentation(indentation_),
    _rest_of_lines_indentation(indentation_)
{
}

paragraph::paragraph(const std::string& content_,
                     const std::string& rest_of_lines_indentation_,
                     const std::string& first_line_indentation_)
  : _content(content_),
    _first_line_indentation(first_line_indentation_),
    _rest_of_lines_indentation(rest_of_lines_indentation_)
{
}

const std::string& paragraph::content() const { return _content; }

const std::string& paragraph::first_line_indentation() const
{
  return _first_line_indentation;
}

const std::string& paragraph::rest_of_lines_indentation() const
{
  return _rest_of_lines_indentation;
}

std::ostream& metashell_system_test::operator<<(std::ostream& out_,
                                                const paragraph& p_)
{
  return out_ << to_json_string(p_);
}

json_string metashell_system_test::to_json_string(const paragraph& p_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  write(p_, w);

  return json_string(buff.GetString());
}
