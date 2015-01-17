// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/paragraph.hpp>
#include <iostream>

using namespace metashell::data;

paragraph::paragraph(
  const std::string& content_,
  const std::string& indentation_
) :
  first_line_indentation(indentation_),
  rest_of_lines_indentation(indentation_),
  content(content_)
{}

paragraph::paragraph(
  const std::string& content_,
  const std::string& indentation_,
  const std::string& first_line_indentation_
) :
  first_line_indentation(first_line_indentation_),
  rest_of_lines_indentation(indentation_),
  content(content_)
{}

bool paragraph::operator==(const paragraph& p_) const
{
  return
    content == p_.content
    && first_line_indentation == p_.first_line_indentation
    && rest_of_lines_indentation == p_.rest_of_lines_indentation;
}

std::ostream& metashell::data::operator<<(std::ostream& o_, const paragraph& p_)
{
  return
    o_
      << "paragraph(\"" << p_.content << "\", \""
      << p_.rest_of_lines_indentation << "\", \"" << p_.first_line_indentation
      << "\")";
}

