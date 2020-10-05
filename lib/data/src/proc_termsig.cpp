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

#include <metashell/data/proc_termsig.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    proc_termsig::proc_termsig(data::signal signal_, bool core_dump_)
      : _signal(signal_), _core_dump(core_dump_)
    {
    }

    data::signal proc_termsig::signal() const { return _signal; }

    bool proc_termsig::core_dump() const { return _core_dump; }

    bool operator==(const proc_termsig& lhs_, const proc_termsig& rhs_)
    {
      return lhs_.signal() == rhs_.signal() &&
             lhs_.core_dump() == rhs_.core_dump();
    }

    std::string to_string(const proc_termsig& p_)
    {
      return p_.core_dump() ? "Process core dumped due to unhandled signal " +
                                  to_string(p_.signal()) :
                              "Process terminated due to unhandled signal " +
                                  to_string(p_.signal());
    }

    std::ostream& operator<<(std::ostream& out_, const proc_termsig& p_)
    {
      return out_ << to_string(p_);
    }
  } // namespace data
} // namespace metashell
