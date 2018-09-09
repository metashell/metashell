#ifndef METASHELL_COLUMN_HPP
#define METASHELL_COLUMN_HPP

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

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace core
  {
    class column : boost::equality_comparable<column>
    {
    public:
      explicit column(int value_);

      int value() const;

      column& operator+=(int n_);
      column& operator-=(int n_);

    private:
      int _value;

      bool invariant() const;
    };

    std::string to_string(column l_);

    std::ostream& operator<<(std::ostream& out_, column l_);

    bool operator==(column a_, column b_);

    inline column operator+(column a_, int b_) { return a_ += b_; }
    inline column operator+(int a_, column b_) { return b_ += a_; }
    inline column operator-(column a_, int b_) { return a_ -= b_; }
  }
}

#endif
