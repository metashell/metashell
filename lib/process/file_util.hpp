#ifndef METASHELL_PROCESS_FILE_UTIL_HPP
#define METASHELL_PROCESS_FILE_UTIL_HPP

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

#include <metashell/process/file.hpp>
#include <metashell/process/output_file.hpp>

namespace metashell
{
  namespace process
  {
#ifndef _WIN32
    template <class UniqueType>
    void use_as(file<UniqueType>& file_, fd_t fd_)
    {
      dup2(file_.fd(), fd_);
    }
#endif

#ifdef _WIN32
    template <class UniqueType>
    fd_t copy_handle(const file<UniqueType>& file_)
    {
      const HANDLE cp = GetCurrentProcess();
      HANDLE r = NULL;
      if (DuplicateHandle(
              cp, file_.fd(), cp, &r, 0, TRUE, DUPLICATE_SAME_ACCESS))
      {
        return r;
      }
      else
      {
        return NULL;
      }
    }
#endif

    void close_on_exec(output_file& file_);
  }
}

#endif
