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

#include <metashell/data/text.hpp>

#include <ostream>

namespace metashell
{
  namespace data
  {
    text::text(const std::string& s_) : paragraphs(1, paragraph(s_)) {}

    text::text(const std::initializer_list<paragraph>& paragraphs_)
      : paragraphs(paragraphs_)
    {
    }

    bool text::operator==(const text& t_) const
    {
      return paragraphs == t_.paragraphs;
    }

    std::ostream& operator<<(std::ostream& o_, const text& t_)
    {
      o_ << "text({";
      bool first = true;
      for (const auto& p : t_.paragraphs)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          o_ << ", ";
        }
        o_ << p;
      }
      return o_ << "})";
    }
  }
}
