#ifndef PATTERN_BASIC_REGEX_HPP
#define PATTERN_BASIC_REGEX_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

namespace pattern
{
  template <class CharT>
  class basic_regex
  {
  public:
    /* implicit */ basic_regex(
        std::basic_string<CharT> exp_ = std::basic_string<CharT>())
      : _exp(exp_)
    {
    }

    const std::basic_string<CharT>& value() const { return _exp; }

  private:
    std::basic_string<CharT> _exp;
  };
} // namespace pattern

#endif
