#ifndef METASHELL_UNSAVED_FILE_HPP
#define METASHELL_UNSAVED_FILE_HPP

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

#include <clang-c/Index.h>

#include <string>

namespace metashell
{
  class unsaved_file
  {
  public:
    explicit unsaved_file(
      const std::string& filename_ = "",
      const std::string& content_ = ""
    );

    // The resulting CXUnsavedFile object refers back to this object.
    // Destroying the unsaved_file object invalidates the CXUnsavedFile.
    CXUnsavedFile get() const;

    const std::string& filename() const;
    const std::string& content() const;

    void generate() const;
  private:
    std::string _filename;
    std::string _content;
  };
}

#endif

