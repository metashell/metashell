#ifndef METASHELL_PROCESS_OUTPUT_FILE_HPP
#define METASHELL_PROCESS_OUTPUT_FILE_HPP

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

#include <string>

namespace metashell
{
  namespace process
  {
    class output_file : public file<output_file>
    {
    public:
      explicit output_file(fd_t fd_);

      size_type write(const char* buff_, size_t count_);

      size_type write(const std::string& s_);
    };
  }
}

#endif
