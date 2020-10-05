#ifndef METASHELL_FOR_EACH_LINE_HPP
#define METASHELL_FOR_EACH_LINE_HPP

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

#include <string>

namespace metashell
{
  namespace core
  {
    template <class F>
    void for_each_line(const std::string& s_, F f_)
    {
      if (!s_.empty())
      {
        bool was_13 = false;
        auto line_start = s_.begin();
        auto line_end = s_.begin();
        for (auto i = s_.begin(), e = s_.end(); i != e; ++i)
        {
          switch (*i)
          {
          case 10:
            f_(std::string(line_start, line_end));
            line_start = i;
            ++line_start;
            line_end = line_start;
            was_13 = false;
            break;
          case 13:
            if (was_13)
            {
              f_(std::string(line_start, line_end));
              line_start = i;
              ++line_start;
              line_end = line_start;
            }
            else
            {
              was_13 = true;
            }
            break;
          default:
            if (was_13)
            {
              f_(std::string(line_start, line_end));
              line_start = i;
              line_end = line_start;
              ++line_end;
              was_13 = false;
            }
            else
            {
              line_end = i;
              ++line_end;
            }
          }
        }
        f_(std::string(line_start, line_end));
        if (was_13)
        {
          f_(std::string());
        }
      }
    }
  } // namespace core
} // namespace metashell

#endif
