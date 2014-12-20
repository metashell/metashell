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

#include <metashell/frame.hpp>

#include <iostream>
#include <sstream>
#include <cassert>

using namespace metashell;

frame::frame(const type& name_) :
  _name(name_)
{}

frame::frame(const type& name_, instantiation_kind kind_) :
  _name(name_),
  _kind(kind_)
{}

const type& frame::name() const
{
  return _name;
}

bool frame::has_kind() const
{
  return bool(_kind);
}

instantiation_kind frame::kind() const
{
  assert(has_kind());
  return *_kind;
}

std::ostream& metashell::operator<<(std::ostream& o_, const frame& f_)
{
  o_ << "frame(\"" << f_.name() << "\"";
  if (f_.has_kind())
  {
    o_ << ", " << f_.kind();
  }
  o_ << ")";
  return o_;
}

bool metashell::operator==(const frame& a_, const frame& b_)
{
  return
    a_.name() == b_.name()
    && a_.has_kind() == b_.has_kind()
    && (!a_.has_kind() || a_.kind() == b_.kind());
}

