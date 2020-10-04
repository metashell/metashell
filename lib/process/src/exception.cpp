// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/process/exception.hpp>

#include <errno.h>
#include <string.h>

namespace metashell
{
  namespace process
  {
    exception::exception(const std::string& reason_)
      : std::runtime_error(reason_)
    {
    }

    exception exception::from_errno() { return from_errno(errno); }

    exception exception::from_errno(int errno_)
    {
      return exception{strerror(errno_)};
    }
  } // namespace process
} // namespace metashell
