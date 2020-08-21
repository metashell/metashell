#ifndef METASHELL_DATA_INDENTER_HPP
#define METASHELL_DATA_INDENTER_HPP

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

#include <sstream>
#include <string>

namespace metashell
{
  namespace data
  {
    class indenter
    {
    public:
      explicit indenter(int width_, std::string line_sep_ = "\n");

      indenter& left_align(const std::string& s_,
                           const std::string& line_prefix_,
                           const std::string& first_line_prefix_);

      indenter& raw(const std::string& s_);

      std::string str() const;

    private:
      int _width;
      std::string _line_sep;
      std::ostringstream _buff;
    };
  }
}

#endif
