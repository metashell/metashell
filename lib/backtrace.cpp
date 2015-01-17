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

#include <metashell/data/backtrace.hpp>

#include <boost/range/algorithm/equal.hpp>

using namespace metashell::data;

backtrace::backtrace(const std::initializer_list<frame>& frames_) :
  _frames(frames_)
{}

void backtrace::push_back(const frame& f_)
{
  _frames.push_back(f_);
}

backtrace::iterator backtrace::begin() const
{
  return _frames.begin();
}

backtrace::iterator backtrace::end() const
{
  return _frames.end();
}

std::ostream& metashell::data::operator<<(std::ostream& o_, const backtrace& t_)
{
  o_ << "backtrace{";
  bool first = true;
  for (const frame& f : t_)
  {
    if (first)
    {
      first = false;
    }
    else
    {
      o_ << ", ";
    }
    o_ << f;
  }
  return o_ << "}";
}

bool metashell::data::operator==(const backtrace& a_, const backtrace& b_)
{
  return boost::equal(a_, b_);
}

