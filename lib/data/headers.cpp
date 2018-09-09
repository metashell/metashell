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

#include <metashell/data/headers.hpp>

namespace metashell
{
  namespace data
  {
    headers::headers(const boost::filesystem::path& internal_dir_)
      : _headers(), _internal_dir(internal_dir_)
    {
    }

    void headers::add(const boost::filesystem::path& filename_,
                      const std::string& content_)
    {
      _headers.push_back(unsaved_file(filename_, content_));
    }

    headers::iterator headers::begin() const { return _headers.begin(); }

    headers::iterator headers::end() const { return _headers.end(); }

    headers::size_type headers::size() const { return _headers.size(); }

    const boost::filesystem::path& headers::internal_dir() const
    {
      return _internal_dir;
    }
  }
}
