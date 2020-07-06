#ifndef METASHELL_MAIN_SHELL_METASHELL_PRAGMA_HPP
#define METASHELL_MAIN_SHELL_METASHELL_PRAGMA_HPP

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

#include <metashell/data/command.hpp>

#include <optional>
#include <string>

namespace metashell
{
  namespace main_shell
  {
    std::optional<data::command::iterator>
    parse_pragma(const data::command& cmd_);

    data::command::iterator
    end_of_pragma_argument_list(data::command::iterator begin_,
                                const data::command::iterator& end_);
  }
}

#endif
