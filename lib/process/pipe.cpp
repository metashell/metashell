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

#include "pipe.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace metashell
{
  namespace process
  {
    pipe::fds::fds()
    {
#ifdef _WIN32
      CreatePipe(_fd, _fd + 1, NULL, 0);
#else
      ::pipe(_fd);
#endif
    }

    fd_t pipe::fds::read_fd() const { return _fd[0]; }
    fd_t pipe::fds::write_fd() const { return _fd[1]; }

    pipe::pipe(fds fds_) : input(fds_.read_fd()), output(fds_.write_fd()) {}
  }
}
