// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command_line_argument.hpp>

#include <boost/algorithm/string/predicate.hpp>

namespace metashell
{
  namespace data
  {
    command_line_argument::command_line_argument(
        const boost::filesystem::path& path_)
      : string<command_line_argument>(path_.string())
    {
    }

    command_line_argument::command_line_argument(const char* value_)
      : string<command_line_argument>(value_)
    {
    }

    bool command_line_argument::starts_with_impl(const char* prefix_) const
    {
      return boost::starts_with(value(), prefix_);
    }

    std::string quote(const command_line_argument& arg_)
    {
      return arg_.value();
    }
  }
}
