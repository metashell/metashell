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

#include <metashell/system_test/code_completer.hpp>
#include <metashell/system_test/code_completion_result.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>

using pattern::_;

namespace metashell
{
  namespace system_test
  {
    code_completer::code_completer(std::string init_code_,
                                   data::command_line_argument_list extra_args_,
                                   boost::filesystem::path cwd_)
      : _init_code{std::move(init_code_)},
        _extra_args{std::move(extra_args_)},
        _cwd{std::move(cwd_)}
    {
    }

    json_string code_completer::operator()(const std::string& code_) const
    {
      metashell_instance mi{_extra_args, _cwd};

      for (const json_string& init_result : mi.command(_init_code))
      {
        if (error(_) == init_result)
        {
          return init_result;
        }
      }

      return mi.code_completion(code_).front();
    }
  }
}
