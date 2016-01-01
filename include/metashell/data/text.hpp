#ifndef METASHELL_TEXT_HPP
#define METASHELL_TEXT_HPP

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

#include <boost/operators.hpp>

#include <vector>
#include <iosfwd>

namespace metashell
{
  namespace data
  {
    class text : boost::equality_comparable<text>
    {
    public:
      text() = default;
      explicit text(const std::string& s_);
      explicit text(const std::initializer_list<paragraph>& paragraphs_);

      bool operator==(const text& t_) const;

      std::vector<paragraph> paragraphs;
    };

    std::ostream& operator<<(std::ostream& o_, const text& t_);
  }
}

#endif
