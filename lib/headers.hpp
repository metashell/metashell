#ifndef METASHELL_HEADERS_HPP
#define METASHELL_HEADERS_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <boost/iterator/transform_iterator.hpp>

#include <map>
#include <vector>
#include <string>

namespace metashell
{
  class headers
  {
  private:
    typedef std::pair<std::string, std::string> header;
  public:
    headers(const std::string& src_, const std::string& internal_dir_);

    typedef
      boost::transform_iterator<
        CXUnsavedFile (*)(const header&),
        std::vector<header>::const_iterator
      >
      iterator;

    iterator begin() const;
    iterator end() const;

    std::string operator[](const std::string& filename_) const;
  private:
    std::vector<header> _headers;

    void add(const std::string& filename_, const std::string& content_);
  };
}

#endif

