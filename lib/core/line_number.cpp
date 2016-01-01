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

#include <metashell/line_number.hpp>

#include <cassert>
#include <iostream>

using namespace metashell;

bool line_number::invariant() const { return _value > 0; }

line_number::line_number(int value_) : _value(value_) { assert(invariant()); }

int line_number::value() const { return _value; }

line_number& line_number::operator+=(int n_)
{
  _value += n_;
  assert(invariant());
  return *this;
}

line_number& line_number::operator-=(int n_)
{
  _value -= n_;
  assert(invariant());
  return *this;
}

std::string metashell::to_string(line_number l_)
{
  return std::to_string(l_.value());
}

std::ostream& metashell::operator<<(std::ostream& out_, line_number l_)
{
  return out_ << to_string(l_);
}

bool metashell::operator==(line_number a_, line_number b_)
{
  return a_.value() == b_.value();
}
