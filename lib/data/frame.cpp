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

#include <metashell/data/frame.hpp>

#include <ostream>
#include <sstream>
#include <cassert>

using namespace metashell::data;

frame::frame(const type& name_) : _name(name_) {}

frame::frame(
    const type& name_,
    const file_location& point_of_instantiation_,
    instantiation_kind kind_) :
  _name(name_),
  _point_of_instantiation(point_of_instantiation_),
  _kind(kind_)
{}

const type& frame::name() const
{
  return _name;
}

bool frame::is_full() const
{
  assert(bool(_kind) == bool(_point_of_instantiation));
  return bool(_kind);
}

instantiation_kind frame::kind() const
{
  assert(is_full());
  return *_kind;
}

const file_location& frame::point_of_instantiation() const
{
  assert(is_full());
  return *_point_of_instantiation;
}


std::ostream& metashell::data::operator<<(std::ostream& o_, const frame& f_)
{
  o_ << "frame(\"" << f_.name() << "\"";
  if (f_.is_full())
  {
    o_ << ", " << f_.point_of_instantiation() << ", " << f_.kind();
  }
  o_ << ")";
  return o_;
}

bool metashell::data::operator==(const frame& a_, const frame& b_)
{
  return
    a_.name() == b_.name()
    && a_.is_full() == b_.is_full()
    && (!a_.is_full() ||
        (a_.kind() == b_.kind()
        // TODO commented out, because I don't want to include this information
        // into the UTs.
        /*&& a_.point_of_instantiation() == b_.point_of_instantiation()*/));
}

