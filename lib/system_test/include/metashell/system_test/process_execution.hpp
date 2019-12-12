#ifndef METASHELL_SYSTEM_TEST_PROCESS_EXECUTION_HPP
#define METASHELL_SYSTEM_TEST_PROCESS_EXECUTION_HPP

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

#include <metashell/data/command_line_argument_list.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace system_test
  {
    class process_execution
    {
    public:
      explicit process_execution(data::command_line_argument_list);

      const data::command_line_argument_list& args() const;

      const std::string& standard_input() const;
      std::string& standard_input();

      const std::string& standard_output() const;
      std::string& standard_output();

    private:
      data::command_line_argument_list _args;
      std::string _stdin;
      std::string _stdout;
    };

    std::ostream& operator<<(std::ostream& out_, const process_execution& e_);
  }
}

#endif
