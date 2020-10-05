#ifndef METASHELL_INCLUDE_PATH_CACHE_HPP
#define METASHELL_INCLUDE_PATH_CACHE_HPP

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

#include <metashell/core/cached.hpp>

#include <metashell/data/include_type.hpp>
#include <metashell/data/standard_headers_allowed.hpp>

#include <metashell/iface/header_discoverer.hpp>

#include <boost/filesystem/path.hpp>

#include <map>
#include <vector>

namespace metashell
{
  namespace core
  {
    struct include_path_cache
    {
    public:
      using key_type =
          std::pair<data::include_type, data::standard_headers_allowed>;

      explicit include_path_cache(iface::header_discoverer& header_discoverer_);

      const std::vector<boost::filesystem::path>& operator[](key_type);

    private:
      std::map<key_type, cached<std::vector<boost::filesystem::path>>> _cache;
    };
  } // namespace core
} // namespace metashell

#endif
