// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include "replace_part.hpp"

#include <boost/optional.hpp>

#include <cassert>

namespace replace_part
{
  namespace
  {
    boost::optional<boost::filesystem::path> replace_part_path = boost::none;
  }

  void path(const boost::filesystem::path& path_)
  {
    assert(!replace_part_path);
    replace_part_path = path_;
  }

  metashell::process::execution run(const std::vector<std::string>& args_)
  {
    assert(replace_part_path);
    std::vector<std::string> args{replace_part_path->string()};
    args.insert(args.end(), args_.begin(), args_.end());

    return metashell::process::execution(args);
  }
}
