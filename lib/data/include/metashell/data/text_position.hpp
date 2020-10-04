#ifndef METASHELL_TEXT_POSITION_HPP
#define METASHELL_TEXT_POSITION_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class text_position : boost::equality_comparable<text_position>
    {
    public:
      text_position();
      text_position(int line_, int column_);

      int line() const;
      int column() const;

      void next_column();
      void next_line();

      bool operator==(const text_position& pos_) const;

    private:
      int _line;
      int _column;
    };

    /*
     * Character sequences treated as one new line:
     *   10
     *   13
     *   13 10
     */
    text_position operator+(text_position pos_, const std::string& s_);

    std::ostream& operator<<(std::ostream& o_, const text_position& pos_);
  } // namespace data
} // namespace metashell

#endif
