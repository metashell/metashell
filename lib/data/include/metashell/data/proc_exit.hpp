#ifndef METASHELL_DATA_PROC_EXIT_HPP
#define METASHELL_DATA_PROC_EXIT_HPP

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

#include <metashell/data/exit_status.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class proc_exit : boost::equality_comparable<proc_exit>
    {
    public:
      explicit proc_exit(data::exit_status);

      data::exit_status exit_status() const;

    private:
      data::exit_status _exit_status;
    };

    proc_exit exit_success();
    proc_exit exit_failure();

    bool exit_success(const proc_exit&);

    bool operator==(const proc_exit&, const proc_exit&);

    std::string to_string(const proc_exit&);
    std::ostream& operator<<(std::ostream&, const proc_exit&);
  } // namespace data
} // namespace metashell

#endif
