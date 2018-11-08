#ifndef METASHELL_CORE_IS_ENVIRONMENT_SETUP_COMMAND_HPP
#define METASHELL_CORE_IS_ENVIRONMENT_SETUP_COMMAND_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command.hpp>

namespace metashell
{
  namespace core
  {
    bool is_environment_setup_command(data::command::iterator begin_,
                                      const data::command::iterator& end_);

    inline bool is_environment_setup_command(const data::command& cmd_)
    {
      return is_environment_setup_command(cmd_.begin(), cmd_.end());
    }
  }
}

#endif
