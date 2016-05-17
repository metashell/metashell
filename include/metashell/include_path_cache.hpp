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

#include <metashell/cached.hpp>
#include <metashell/iface/engine.hpp>
#include <metashell/data/include_type.hpp>

#include <boost/filesystem/path.hpp>

#include <vector>

namespace metashell
{
  struct include_path_cache
  {
  public:
    cached<std::vector<boost::filesystem::path>> sys;
    cached<std::vector<boost::filesystem::path>> quote;

    explicit include_path_cache(iface::engine& engine_);

    const std::vector<boost::filesystem::path>&
    operator[](data::include_type type_);
  };
}

#endif
