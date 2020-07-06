// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/util.hpp>

#include <boost/filesystem.hpp>

#include <set>

namespace metashell
{
  namespace core
  {
    boost::filesystem::path resolve_symlink(boost::filesystem::path p_)
    {
      std::set<boost::filesystem::path> visited;

      while (visited.find(p_) == visited.end() && is_symlink(p_))
      {
        visited.insert(p_);
        p_ = read_symlink(p_);
      }

      return p_;
    }
  }
}
