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

#include <metashell/column.hpp>

#include <cassert>
#include <iostream>

using namespace metashell;

bool column::invariant() const { return _value > 0; }

column::column(int value_) : _value(value_) { assert(invariant()); }

int column::value() const { return _value; }

column& column::operator+=(int n_)
{
  _value += n_;
  assert(invariant());
  return *this;
}

column& column::operator-=(int n_)
{
  _value -= n_;
  assert(invariant());
  return *this;
}

std::string metashell::to_string(column l_)
{
  return std::to_string(l_.value());
}

std::ostream& metashell::operator<<(std::ostream& out_, column l_)
{
  return out_ << to_string(l_);
}

bool metashell::operator==(column a_, column b_)
{
  return a_.value() == b_.value();
}
