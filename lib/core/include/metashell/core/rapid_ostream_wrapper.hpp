#ifndef METASHELL_RAPID_OSTREAM_WRAPPER_HPP
#define METASHELL_RAPID_OSTREAM_WRAPPER_HPP

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

#include <iosfwd>

namespace metashell
{
  namespace core
  {
    // Based on RapidJSON's documentation
    // http://miloyip.github.io/rapidjson/md_doc_stream.html#CustomStream
    class rapid_ostream_wrapper
    {
    public:
      typedef char Ch;

      rapid_ostream_wrapper(std::ostream& os_);

      void Put(Ch c_);
      void Flush();

      void new_line();

      Ch Peek() const;
      Ch Take();
      size_t Tell() const;
      Ch* PutBegin();
      size_t PutEnd(Ch*);

    private:
      std::ostream& _os;
    };
  }
}

#endif
