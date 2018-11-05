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

#include "run.hpp"

#include <metashell/main_shell/metashell_pragma.hpp>

#include <metashell/core/command.hpp>

#include <metashell/mock/shell.hpp>

using ::testing::NiceMock;

void run(const metashell::iface::pragma_handler& handler_,
         const metashell::data::cpp_code& args_,
         metashell::iface::displayer& displayer_)
{
  NiceMock<metashell::mock::shell> sh;

  const metashell::data::command cmd = metashell::core::to_command(args_);
  handler_.run(cmd.begin(), cmd.begin(), cmd.begin(),
               metashell::main_shell::end_of_pragma_argument_list(
                   cmd.begin(), cmd.end()),
               sh, displayer_);
}
