#ifndef METASHELL_COMPILER_STUB_STUB_HPP
#define METASHELL_COMPILER_STUB_STUB_HPP

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

#include <metashell/data/process_output.hpp>

#include <map>
#include <string>
#include <vector>

namespace metashell
{
  namespace compiler_stub
  {
    class stub
    {
    public:
      stub& on_args(std::vector<std::string>, data::process_output);

      data::proc_exit run(int argc_, const char* argv_[]) const;

    private:
      std::map<std::vector<std::string>, data::process_output> _actions;
    };
  }
}

#endif
