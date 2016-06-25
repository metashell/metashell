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

#include "input_file.hpp"

#include <cassert>
#include <sstream>

namespace metashell
{
  namespace process
  {
    input_file::input_file(fd_t fd_) : file<input_file>(fd_), _eof(false) {}

#ifdef _WIN32
    input_file::size_type input_file::read(char* buf_, size_t count_)
    {
      DWORD len = 0;
      if (ReadFile(fd(), buf_, count_, &len, NULL))
      {
        _eof = (len == 0);
        return len;
      }
      else
      {
        _eof = true;
        return 0;
      }
    }
#endif

#ifndef _WIN32
    input_file::size_type input_file::read(char* buf_, size_t count_)
    {
      const ssize_t len = ::read(fd(), buf_, count_);
      _eof = (len == 0);
      return len;
    }
#endif

    bool input_file::eof() const { return _eof; }

    std::string input_file::read()
    {
      std::ostringstream s;
      static const int buffsize = 1024;
      char buff[buffsize];
      while (!eof())
      {
        s.write(buff, read(buff, buffsize));
      }
      return s.str();
    }
  }
}
