#ifndef METASHELL_LINE_NUMBER_HPP
#define METASHELL_LINE_NUMBER_HPP

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

#include <string>
#include <iosfwd>

namespace metashell
{
  class line_number : boost::equality_comparable<line_number>
  {
  public:
    explicit line_number(int value_);

    int value() const;

    line_number& operator+=(int n_);
    line_number& operator-=(int n_);

  private:
    int _value;

    bool invariant() const;
  };

  std::string to_string(line_number l_);

  std::ostream& operator<<(std::ostream& out_, line_number l_);

  bool operator==(line_number a_, line_number b_);

  inline line_number operator+(line_number a_, int b_) { return a_ += b_; }
  inline line_number operator+(int a_, line_number b_) { return b_ += a_; }
  inline line_number operator-(line_number a_, int b_) { return a_ -= b_; }
}

#endif
