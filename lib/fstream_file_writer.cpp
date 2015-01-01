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

#include <metashell/fstream_file_writer.hpp>

using namespace metashell;

namespace
{
  bool was_error(const std::ostream& s_)
  {
    return s_.fail() || s_.bad();
  }
}

bool fstream_file_writer::open(const std::string& filename_)
{
  _f.open(filename_.c_str());
  return !was_error(_f);
}

void fstream_file_writer::close()
{
  _f.close();
}

bool fstream_file_writer::is_open() const
{
  return _f.is_open();
}

bool fstream_file_writer::write(const std::string& content_)
{
  _f << content_;
  return !was_error(_f);
}

