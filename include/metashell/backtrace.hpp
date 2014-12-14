#ifndef METASHELL_BACKTRACE_HPP
#define METASHELL_BACKTRACE_HPP

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

#include <boost/operators.hpp>

#include <vector>
#include <iosfwd>

namespace metashell
{
  class backtrace : boost::equality_comparable<backtrace>
  {
  public:
    typedef std::vector<frame>::const_iterator iterator;
    typedef iterator const_iterator;

    backtrace() = default;

    backtrace(const std::initializer_list<frame>& frames_);

    void push_back(const frame& f_);

    iterator begin() const;
    iterator end() const;
  private:
    std::vector<frame> _frames;
  };
  
  std::ostream& operator<<(std::ostream& o_, const backtrace& t_);
  bool operator==(const backtrace& a_, const backtrace& b_);
}

#endif

