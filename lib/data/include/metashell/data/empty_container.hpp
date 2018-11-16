#ifndef METASHELL_DATA_EMPTY_CONTAINER_HPP
#define METASHELL_DATA_EMPTY_CONTAINER_HPP

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

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class empty_container
    {
    };

    std::ostream& operator<<(std::ostream& out_, const empty_container&);

    std::string to_string(const empty_container&);

    template <class T>
    bool operator==(const empty_container&, const T& c_)
    {
      return c_.empty();
    }

    template <class T>
    bool operator==(const T& c_, const empty_container& e_)
    {
      return e_ == c_;
    }

    template <class T>
    bool operator!=(const empty_container& e_, const T& c_)
    {
      return !(e_ == c_);
    }

    template <class T>
    bool operator!=(const T& c_, const empty_container& e_)
    {
      return !(e_ == c_);
    }
  }
}

#endif
