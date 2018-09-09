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

#include <metashell/core/metashell.hpp>
#include <metashell/core/pragma_environment_add.hpp>
#include <metashell/core/shell.hpp>

namespace metashell
{
  namespace core
  {
    pragma_environment_add::pragma_environment_add(shell& shell_)
      : _shell(shell_)
    {
    }

    iface::pragma_handler* pragma_environment_add::clone() const
    {
      return new pragma_environment_add(_shell);
    }

    std::string pragma_environment_add::arguments() const { return "<code>"; }

    std::string pragma_environment_add::description() const
    {
      return "Appends code to the environment. Use this if Metashell thinks "
             "about "
             "the"
             " code that it is an evaluation.";
    }

    void pragma_environment_add::run(const data::command::iterator&,
                                     const data::command::iterator&,
                                     const data::command::iterator& args_begin_,
                                     const data::command::iterator& args_end_,
                                     iface::displayer& displayer_) const
    {
      const data::cpp_code cmd = tokens_to_string(args_begin_, args_end_);

      _shell.store_in_buffer(cmd, displayer_);

      if (is_environment_setup_command(args_begin_, args_end_))
      {
        displayer_.show_comment(data::text(
            "You don't need the environment add pragma to add this to the"
            " environment. The following command does this as well:"));
        displayer_.show_cpp_code(cmd);
      }
      else
      {
        displayer_.show_comment(data::text(
            "Metashell (incorrectly) thinks that this command should execute a"
            " metaprogram and would not add it to the environment without using"
            " the \"environment add\" pragma. Please file a bug report "
            "containing"
            " this command (" +
            cmd.value() +
            ") at"
            " https://github.com/metashell/metashell/issues. Thank you."));
      }
    }
  }
}
