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

#include <metashell/data/text_position.hpp>
#include <ostream>

namespace metashell
{
  namespace data
  {
    text_position::text_position() : _line(1), _column(1) {}

    text_position::text_position(int line_, int column_)
      : _line(line_), _column(column_)
    {
    }

    int text_position::line() const { return _line; }

    int text_position::column() const { return _column; }

    void text_position::next_column() { ++_column; }

    void text_position::next_line()
    {
      _column = 1;
      ++_line;
    }

    text_position operator+(text_position pos_, const std::string& s_)
    {
      bool was13 = false;

      for (char c : s_)
      {
        switch (c)
        {
        case 10:
          pos_.next_line();
          break;
        case 13:
          if (was13)
          {
            pos_.next_line();
          }
          break;
        default:
          pos_.next_column();
        }
        was13 = c == 13;
      }

      if (was13)
      {
        pos_.next_line();
      }

      return pos_;
    }

    bool text_position::operator==(const text_position& pos_) const
    {
      return _line == pos_._line && _column == pos_._column;
    }

    std::ostream& operator<<(std::ostream& o_, const text_position& pos_)
    {
      return o_ << pos_.line() << ":" << pos_.column();
    }
  }
}
