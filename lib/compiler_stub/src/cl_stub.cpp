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

#include <metashell/compiler_stub/cl_stub.hpp>

namespace metashell
{
  namespace compiler_stub
  {
    cl_stub::cl_stub(
        std::string version_,
        std::string usage_,
        const std::function<std::string(const std::string&)>& invalid_arg_)
    {
      _stub.on_args({}, {data::exit_code_t(1), usage_, version_});

      for (const std::string& arg : std::vector<std::string>{"-v", "--help"})
      {
        _stub.on_args(
            {arg}, {data::exit_code_t(1), "", version_ + invalid_arg_(arg)});
      }
    }

    data::exit_code_t cl_stub::run(int argc_, const char* argv_[]) const
    {
      return _stub.run(argc_, argv_);
    }
  }
}
