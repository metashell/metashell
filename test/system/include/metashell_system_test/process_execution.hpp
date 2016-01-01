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

#include <vector>
#include <string>
#include <iosfwd>

namespace metashell_system_test
{
  class process_execution
  {
  public:
    process_execution(std::vector<std::string> cmd_,
                      std::string stdin_,
                      std::string stdout_,
                      std::string stderr_,
                      int exit_code_);

    const std::vector<std::string>& cmd() const;
    const std::string& standard_input() const;

    const std::string& standard_output() const;
    const std::string& standard_error() const;
    int exit_code() const;

  private:
    std::vector<std::string> _cmd;
    std::string _stdin;
    std::string _stdout;
    std::string _stderr;
    int _exit_code;
  };

  process_execution run(std::vector<std::string> cmd_, std::string input_);

  std::ostream& operator<<(std::ostream& out_, const process_execution& e_);
}

#endif
