#ifndef METASHELL_PROCESS_FILE_HPP
#define METASHELL_PROCESS_FILE_HPP

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

#include <metashell/process/fd_t.hpp>

#include <cassert>

#ifndef _WIN32
#include <sys/types.h>
#include <unistd.h>
#endif

namespace metashell
{
  namespace process
  {
    // Note that this is not CRTP. It is a template class to avoid different
    // classes using it having a common base class (they inherit from
    // different template instances)
    template <class UniqueType>
    class file
    {
    public:
#ifdef _WIN32
      typedef DWORD size_type;
#else
      typedef ssize_t size_type;
#endif
      explicit file(fd_t fd_) : _fd(fd_) {}

      file(const file&) = delete;
      file& operator=(const file&) = delete;

      // Adding move operations for implementing ownership transfer
      file(file&& f_) : _fd(f_._fd) { f_._fd = invalid_fd(); }

      file& operator=(file&& f_)
      {
        if (this != &f_)
        {
          close();
          _fd = f_._fd;
          f_._fd = invalid_fd();
        }
        return *this;
      }

      ~file() { close(); }

      fd_t fd() const { return _fd; }

      void close()
      {
        if (_fd != invalid_fd())
        {
#ifdef _WIN32
          CloseHandle(_fd);
#else
          ::close(_fd);
#endif
          _fd = invalid_fd();
        }
      }

    private:
      fd_t _fd;
    };
  }
}

#endif
