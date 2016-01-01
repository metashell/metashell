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

#include "mock_file_writer.hpp"

mock_file_writer::mock_file_writer()
  : open_callback([](const std::string&)
                  {
                    return false;
                  }),
    close_callback([]
                   {
                   }),
    is_open_callback([]
                     {
                       return false;
                     }),
    write_callback([](const std::string&)
                   {
                     return false;
                   })
{
}

bool mock_file_writer::open(const std::string& filename_)
{
  return open_callback(filename_);
}

void mock_file_writer::close() { close_callback(); }

bool mock_file_writer::is_open() const { return is_open_callback(); }

bool mock_file_writer::write(const std::string& content_)
{
  return write_callback(content_);
}
