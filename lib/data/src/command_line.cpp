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

#include <metashell/data/command_line.hpp>

namespace metashell
{
  namespace data
  {
    command_line::command_line(executable_path exe_,
                               command_line_argument_list args_)
      : _exe(std::move(exe_)), _args(std::move(args_))
    {
    }

    std::vector<const char*> command_line::argv() const
    {
      return _args.argv(_exe);
    }

    const executable_path& command_line::executable() const { return _exe; }

    const command_line_argument_list& command_line::arguments() const
    {
      return _args;
    }

    std::ostream& operator<<(std::ostream& out_, const command_line& c_)
    {
      return out_ << to_string(c_);
    }

    std::string to_string(const command_line& c_)
    {
      return data::to_string(c_.executable()) + " " +
             data::to_string(c_.arguments());
    }
  }
}
