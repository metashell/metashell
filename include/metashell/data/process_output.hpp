#ifndef METASHELL_DATA_PROCESS_OUTPUT_HPP
#define METASHELL_DATA_PROCESS_OUTPUT_HPP

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

#include <metashell/data/exit_code_t.hpp>

#include <string>

namespace metashell
{
  namespace data
  {
    class process_output
    {
    public:
      process_output(
        exit_code_t exit_code_,
        const std::string& stdout_,
        const std::string& stderr_
      );

      exit_code_t exit_code() const;
      const std::string& standard_output() const;
      const std::string& standard_error() const;
    private:
      exit_code_t _exit_code;
      std::string _out;
      std::string _err;
    };
  }
}

#endif

