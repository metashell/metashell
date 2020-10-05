// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/proc_stopsig.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    proc_stopsig::proc_stopsig(data::signal signal_) : _signal(signal_) {}

    data::signal proc_stopsig::signal() const { return _signal; }

    bool operator==(const proc_stopsig& lhs_, const proc_stopsig& rhs_)
    {
      return lhs_.signal() == rhs_.signal();
    }

    std::string to_string(const proc_stopsig& p_)
    {
      return "Process stopped by signal " + to_string(p_.signal());
    }

    std::ostream& operator<<(std::ostream& out_, const proc_stopsig& p_)
    {
      return out_ << to_string(p_);
    }
  } // namespace data
} // namespace metashell
