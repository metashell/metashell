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

#include <metashell/engine/vc/preprocessor_shell.hpp>

namespace metashell
{
  namespace engine
  {
    namespace vc
    {
      preprocessor_shell::preprocessor_shell(binary binary_) : _binary(binary_)
      {
      }

      data::result preprocessor_shell::precompile(const data::cpp_code& exp_)
      {
        const data::process_output output = run(_binary, {"/E"}, exp_);

        const bool success = exit_success(output);

        return data::result{success, success ? output.standard_output : "",
                            success ? "" : error_report_on_stderr(output), ""};
      }
    } // namespace vc
  } // namespace engine
} // namespace metashell
