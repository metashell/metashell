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

#include <metashell/process/output_file.hpp>

#include <cassert>

#ifndef _WIN32
#include <unistd.h>
#endif

namespace metashell
{
  namespace process
  {
    output_file::output_file(fd_t fd_) : file<output_file>(fd_) {}

    output_file::output_file(output_file&& f_)
      : file<output_file>(std::move(f_))
    {
    }

    output_file& output_file::operator=(output_file&& f_)
    {
      file<output_file>::operator=(std::move(f_));
      return *this;
    }

#ifdef _WIN32
    output_file::size_type output_file::write(const char* buff_, size_t count_)
    {
      DWORD len = 0;
      if (WriteFile(fd(), buff_, count_, &len, NULL))
      {
        return len;
      }
      else
      {
        return 0;
      }
    }
#endif

#ifndef _WIN32
    output_file::size_type output_file::write(const char* buff_, size_t count_)
    {
      return ::write(fd(), buff_, count_);
    }
#endif

    output_file::size_type output_file::write(const std::string& s_)
    {
      return write(s_.c_str(), s_.length());
    }
  }
}
