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

#include <metashell/engine/clang/this_engine.hpp>

#include <metashell/process/exception.hpp>
#include <metashell/process/run.hpp>

#include <regex>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      bool is_clang(const data::executable_path& clang_)
      {
        try
        {
          return regex_search(
              process::run(data::command_line(clang_, {"-v"}), "")
                  .standard_error,
              std::regex("^(clang version |Apple LLVM version |Apple clang "
                         "version )"));
        }
        catch (const process::exception&)
        {
          return false;
        }
      }

      bool is_templight(const data::executable_path& clang_)
      {
        try
        {
          return regex_search(
              process::run(data::command_line(clang_, {"--help"}), "")
                  .standard_output,
              std::regex("-Xtemplight "));
        }
        catch (const process::exception&)
        {
          return false;
        }
      }
    }
  }
}
