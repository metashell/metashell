#ifndef METASHELL_DATA_STATUS_HPP
#define METASHELL_DATA_STATUS_HPP

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

#include <metashell/data/proc_exit.hpp>
#include <metashell/data/proc_stopsig.hpp>
#include <metashell/data/proc_termsig.hpp>

#include <variant.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    using status = mpark::variant<proc_exit, proc_termsig, proc_stopsig>;

    bool exit_success(const status&);

    std::string to_string(const status&);
    std::ostream& operator<<(std::ostream&, const status&);
  } // namespace data
} // namespace metashell

#endif
