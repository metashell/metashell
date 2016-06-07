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

#include <metashell/for_each_line.hpp>
#include <metashell/source_position.hpp>

#include <iostream>

using namespace metashell;

source_position::source_position() : _line(1), _column(1) {}

source_position::source_position(line_number line_, column column_)
  : _line(line_), _column(column_)
{
}

line_number source_position::line() const { return _line; }

column source_position::col() const { return _column; }

std::string metashell::to_string(const source_position& p_)
{
  return to_string(p_.line()) + ":" + to_string(p_.col());
}

std::ostream& metashell::operator<<(std::ostream& out_,
                                    const source_position& p_)
{
  return out_ << to_string(p_);
}

bool metashell::operator==(const source_position& a_, const source_position& b_)
{
  return a_.line() == b_.line() && a_.col() == b_.col();
}

source_position metashell::source_position_of(const std::string& s_)
{
  source_position result;
  bool first_line = true;
  for_each_line(s_, [&result, &first_line](const std::string& line_) {
    if (first_line)
    {
      result = source_position(result.line(), column(line_.length() + 1));
      first_line = false;
    }
    else
    {
      result = source_position(result.line() + 1, column(line_.length() + 1));
    }
  });
  return result;
}
