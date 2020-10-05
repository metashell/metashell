#ifndef METASHELL_DATA_PROC_STOPSIG_HPP
#define METASHELL_DATA_PROC_STOPSIG_HPP

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

#include <metashell/data/signal.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class proc_stopsig : boost::equality_comparable<proc_stopsig>
    {
    public:
      explicit proc_stopsig(data::signal);

      data::signal signal() const;

    private:
      data::signal _signal;
    };

    bool operator==(const proc_stopsig&, const proc_stopsig&);

    std::string to_string(const proc_stopsig&);
    std::ostream& operator<<(std::ostream&, const proc_stopsig&);
  } // namespace data
} // namespace metashell

#endif
