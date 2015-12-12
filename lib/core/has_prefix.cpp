// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/has_prefix.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  bool has_prefix(
    const std::string& arg_,
    const std::vector<std::string>& prefixes_
  )
  {
    return
      std::any_of(
        prefixes_.begin(),
        prefixes_.end(),
        [&arg_](const std::string& prefix_)
        {
          return boost::algorithm::starts_with(arg_, prefix_);
        }
      );
  }
}

bool metashell::has_prefix(
  const std::vector<std::string>& args_,
  const std::vector<std::string>& prefixes_
)
{
  return
    std::any_of(
      args_.begin(),
      args_.end(),
      [&prefixes_](const std::string& arg_)
      {
        return ::has_prefix(arg_, prefixes_);
      }
    );
}

