#ifndef METASHELL_SOURCE_POSITION_HPP
#define METASHELL_SOURCE_POSITION_HPP

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

#include <boost/operators.hpp>
#include <metashell/line_number.hpp>
#include <metashell/column.hpp>

#include <string>
#include <iosfwd>

namespace metashell
{
  class source_position : boost::equality_comparable<source_position>
  {
  public:
    source_position();
    source_position(line_number line_, column column_);

    line_number line() const;
    column col() const;

  private:
    line_number _line;
    column _column;
  };

  std::string to_string(const source_position& p_);

  std::ostream& operator<<(std::ostream& out_, const source_position& p_);

  bool operator==(const source_position& a_, const source_position& b_);

  source_position source_position_of(const std::string& s_);
}

#endif
