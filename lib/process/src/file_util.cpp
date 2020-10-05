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

#include "file_util.hpp"

#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#endif

namespace metashell
{
  namespace process
  {
    void close_on_exec(output_file& file_)
    {
#ifndef _WIN32
      fcntl(file_.fd(), F_SETFD, fcntl(file_.fd(), F_GETFD) | FD_CLOEXEC);
#endif
    }
  } // namespace process
} // namespace metashell
