#ifndef PATTERN_STRING_HPP
#define PATTERN_STRING_HPP

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

#include <pattern/basic_string.hpp>

namespace pattern
{
  typedef basic_string<char> string;

  template <class Writer>
  void write(const string& pattern_, Writer& writer_)
  {
    if (const std::optional<std::string> value = pattern_.value())
    {
      writer_.String(value->c_str());
    }
    else
    {
      writer_.Null();
    }
  }
}

#endif
