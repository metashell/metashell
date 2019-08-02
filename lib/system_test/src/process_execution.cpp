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

#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/process_execution.hpp>

#include <iostream>

namespace metashell
{
  namespace system_test
  {
    process_execution::process_execution(data::command_line_argument_list args_)
      : _args(std::move(args_)), _stdin(), _stdout()
    {
    }

    const data::command_line_argument_list& process_execution::args() const
    {
      return _args;
    }

    const std::string& process_execution::standard_input() const
    {
      return _stdin;
    }
    std::string& process_execution::standard_input() { return _stdin; }

    const std::string& process_execution::standard_output() const
    {
      return _stdout;
    }

    std::string& process_execution::standard_output() { return _stdout; }

    std::ostream& operator<<(std::ostream& out_, const process_execution& e_)
    {
      return out_ << "Command: " << to_json(e_.args()) << std::endl
                  << std::endl
                  << "Standard input: " << to_json(e_.standard_input())
                  << std::endl
                  << std::endl
                  << "Standard output: " << to_json(e_.standard_output())
                  << std::endl;
    }
  }
}
