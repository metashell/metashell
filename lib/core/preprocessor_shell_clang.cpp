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

#include <metashell/preprocessor_shell_clang.hpp>

namespace metashell
{
  preprocessor_shell_clang::preprocessor_shell_clang(clang_binary clang_binary_)
    : _clang_binary(clang_binary_)
  {
  }

  data::result preprocessor_shell_clang::precompile(const data::cpp_code& exp_)
  {
    const data::process_output output = run_clang(_clang_binary, {"-E"}, exp_);

    const bool success = output.exit_code == data::exit_code_t(0);

    return data::result{success, success ? output.standard_output : "",
                        success ? "" : output.standard_error, ""};
  }
}
