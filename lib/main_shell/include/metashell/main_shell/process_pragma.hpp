#ifndef METASHELL_MAIN_SHELL_PROCESS_PRAGMA_HPP
#define METASHELL_MAIN_SHELL_PROCESS_PRAGMA_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/pragma_handler.hpp>
#include <metashell/iface/shell.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace metashell
{
  namespace main_shell
  {
    void process_pragma(
        const std::map<std::vector<std::string>,
                       std::unique_ptr<iface::pragma_handler>>& handlers_,
        const data::command::iterator& begin_,
        const data::command::iterator& end_,
        iface::shell& shell_,
        iface::displayer& displayer_);
  }
}

#endif
