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

#include <metashell/core/include_path_cache.hpp>

#include <cassert>

namespace metashell
{
  namespace core
  {
    include_path_cache::include_path_cache(
        iface::header_discoverer& header_discoverer_)
    {
      for (auto inc : data::all_include_types)
      {
        for (auto allowed : data::all_standard_headers_alloweds)
        {
          _cache.insert({{inc, allowed},
                         cached<std::vector<boost::filesystem::path>>{
                             [&header_discoverer_, inc, allowed] {
                               return header_discoverer_.include_path(
                                   inc, allowed);
                             }}});
        }
      }
    }

    const std::vector<boost::filesystem::path>&
    include_path_cache::operator[](include_path_cache::key_type key_)
    {
      const auto i = _cache.find(key_);
      assert(i != _cache.end());
      return *i->second;
    }
  } // namespace core
} // namespace metashell
