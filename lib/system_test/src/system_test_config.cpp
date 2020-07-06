// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/system_test_config.hpp>

#include <cassert>
#include <optional>

namespace metashell
{
  namespace system_test
  {
    namespace
    {
      std::optional<data::executable_path> metashell_binary_path;
      data::command_line_argument_list metashell_args_impl;
      data::command_line_argument_list engine_args_impl;
    }

    void system_test_config::metashell_binary(data::executable_path path_)
    {
      metashell_binary_path = std::move(path_);
    }

    void system_test_config::metashell_arg(data::command_line_argument arg_)
    {
      metashell_args_impl.push_back(arg_);
      static bool was_dash_dash = false;
      if (was_dash_dash)
      {
        engine_args_impl.push_back(std::move(arg_));
      }
      else if (arg_ == "--")
      {
        was_dash_dash = true;
      }
    }

    data::executable_path system_test_config::metashell_binary()
    {
      assert(metashell_binary_path);
      return *metashell_binary_path;
    }

    const data::command_line_argument_list& system_test_config::metashell_args()
    {
      return metashell_args_impl;
    }

    const data::command_line_argument_list& system_test_config::engine_args()
    {
      return engine_args_impl;
    }
  }
}
