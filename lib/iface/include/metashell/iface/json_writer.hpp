#ifndef METASHELL_IFACE_JSON_WRITER_HPP
#define METASHELL_IFACE_JSON_WRITER_HPP

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
    class json_writer
    {
    public:
      virtual ~json_writer() {}

      virtual void string(const std::string& value_) = 0;
      virtual void int_(int value_) = 0;
      virtual void double_(double value_) = 0;
      virtual void bool_(bool value_) = 0;

      virtual void start_object() = 0;
      virtual void key(const std::string& key_) = 0;
      virtual void end_object() = 0;

      virtual void start_array() = 0;
      virtual void end_array() = 0;

      virtual void end_document() = 0;
    };
  } // namespace iface
} // namespace metashell

#endif
