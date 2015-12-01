#ifndef METASHELL_DATA_HEADERS_HPP
#define METASHELL_DATA_HEADERS_HPP

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

#include <metashell/data/unsaved_file.hpp>

#include <vector>
#include <string>

namespace metashell
{
  namespace data
  {
    class headers
    {
    public:
      explicit headers(const std::string& internal_dir_);

      typedef std::vector<data::unsaved_file>::const_iterator iterator;
      typedef iterator const_iterator;
      typedef std::vector<data::unsaved_file>::size_type size_type;

      iterator begin() const;
      iterator end() const;

      size_type size() const;

      const std::string& internal_dir() const;

      void add(const std::string& filename_, const std::string& content_);
    private:
      std::vector<data::unsaved_file> _headers;
      std::string _internal_dir;
    };
  }
}

#endif

