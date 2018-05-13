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

#include <algorithm>

using namespace metashell::data;

backtrace::backtrace(std::initializer_list<frame> frames_)
  : _frames(std::move(frames_))
{
  std::reverse(_frames.begin(), _frames.end());
}

void backtrace::push_front(const frame& f_) { _frames.push_back(f_); }

void backtrace::pop_front() { _frames.pop_back(); }

bool backtrace::empty() const { return _frames.empty(); }

backtrace::size_type backtrace::size() const { return _frames.size(); }

const frame& backtrace::operator[](size_type i) const
{
  return _frames[_frames.size() - 1 - i];
}

const frame& backtrace::back() const { return _frames.front(); }

backtrace::iterator backtrace::begin() const { return _frames.rbegin(); }

backtrace::iterator backtrace::end() const { return _frames.rend(); }

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
