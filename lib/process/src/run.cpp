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

#include <metashell/process/execution.hpp>
#include <metashell/process/run.hpp>

namespace metashell
{
  namespace process
  {
    data::process_output run(const data::command_line& cmd_,
                             const std::string& input_,
                             const boost::filesystem::path& cwd_)
    {
      execution child(cmd_, cwd_);

      child.standard_input().write(input_);
      child.standard_input().close();

      data::process_output result{data::exit_code_t(0), "", ""};

      read_all(std::tie(child.standard_output(), result.standard_output),
               std::tie(child.standard_error(), result.standard_error));

      result.exit_code = child.wait();

      return result;
    }
  }
}
