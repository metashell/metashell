// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/string.hpp>

#include <cctype>
#include <stdexcept>

namespace metashell
{
  namespace data
  {
    namespace impl
    {
      namespace
      {
        constexpr char hex_digit(int n_)
        {
          switch (n_)
          {
          case 0:
            return '0';
          case 1:
            return '1';
          case 2:
            return '2';
          case 3:
            return '3';
          case 4:
            return '4';
          case 5:
            return '5';
          case 6:
            return '6';
          case 7:
            return '7';
          case 8:
            return '8';
          case 9:
            return '9';
          case 10:
            return 'a';
          case 11:
            return 'b';
          case 12:
            return 'c';
          case 13:
            return 'd';
          case 14:
            return 'e';
          case 15:
            return 'f';
          default:
            return 0;
          }
        }
      }

      std::string c_escape(int c_)
      {
        if (c_ == '\a')
        {
          return "\\a";
        }
        else if (c_ == '\b')
        {
          return "\\b";
        }
        else if (c_ == '\f')
        {
          return "\\f";
        }
        else if (c_ == '\n')
        {
          return "\\n";
        }
        else if (c_ == '\r')
        {
          return "\\r";
        }
        else if (c_ == '\t')
        {
          return "\\t";
        }
        else if (c_ == '\v')
        {
          return "\\v";
        }
        else if (c_ < 32 || 126 < c_)
        {
          return std::string{'\\', 'x', hex_digit(c_ / 16), hex_digit(c_ % 16)};
        }
        else if (c_ == '\\' || c_ == '"' || c_ == '\'')
        {
          return std::string{'\\', char(c_)};
        }
        else
        {
          return std::string{char(c_)};
        }
      }

      std::string to_string(const std::string& s_) { return s_; }

      void throw_(const std::string& what_) { throw std::runtime_error{what_}; }

      bool isspace(int ch_) { return std::isspace(ch_); }
    }
  }
}
