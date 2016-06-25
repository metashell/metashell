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

#include "fd_t.hpp"

#include <boost/noncopyable.hpp>

#include <cassert>

namespace metashell
{
  namespace process
  {
    // Note that this is not CRTP. It is a template class to avoid different
    // classes using it having a common base class (they inherit from
    // different template instances)
    template <class UniqueType>
    class file : boost::noncopyable
    {
    public:
#ifdef _WIN32
      typedef DWORD size_type;
#else
      typedef ssize_t size_type;
#endif
      explicit file(fd_t fd_) : _fd(fd_) {}

      ~file()
      {
        if (_fd != invalid_fd())
        {
          close();
        }
      }

      fd_t fd() { return _fd; }

      void close()
      {
        assert(_fd != invalid_fd());
#ifdef _WIN32
        CloseHandle(_fd);
#else
        ::close(_fd);
#endif
        _fd = invalid_fd();
      }

#ifndef _WIN32
      void use_as(fd_t fd_) { dup2(_fd, fd_); }
#endif

#ifdef _WIN32
      fd_t copy_handle() const
      {
        const HANDLE cp = GetCurrentProcess();
        HANDLE r = NULL;
        if (DuplicateHandle(cp, _fd, cp, &r, 0, TRUE, DUPLICATE_SAME_ACCESS))
        {
          return r;
        }
        else
        {
          return NULL;
        }
      }
#endif

    private:
      fd_t _fd;
    };
  }
}

#endif
