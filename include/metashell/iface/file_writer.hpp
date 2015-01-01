#ifndef METASHELL_IFACE_FILE_WRITER_HPP
#define METASHELL_IFACE_FILE_WRITER_HPP

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

#include <string>

namespace metashell
{
  namespace iface
  {
    class file_writer
    {
    public:
      virtual ~file_writer() {}

      virtual bool open(const std::string& filename_) = 0;
      virtual void close() = 0;
      virtual bool is_open() const = 0;

      virtual bool write(const std::string& content_) = 0;
    };
  }
}

#endif

