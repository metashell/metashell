#ifndef METASHELL_ENVIRONMENT_HPP
#define METASHELL_ENVIRONMENT_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/headers.hpp>

#include <boost/filesystem/path.hpp>

#include <vector>

namespace metashell
{
  namespace iface
  {
    class environment
    {
    public:
      virtual ~environment() {}

      virtual void append(const data::cpp_code& s_) = 0;
      virtual data::cpp_code get() const = 0;
      virtual data::cpp_code get_appended(const data::cpp_code& s_) const = 0;

      virtual const data::headers& get_headers() const = 0;

      // Returns parts that are in precompiled header files as well
      virtual data::cpp_code get_all() const = 0;
    };
  }
}

#endif
