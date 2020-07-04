#ifndef METASHELL_IFACE_PRAGMA_HANDLER_HPP
#define METASHELL_IFACE_PRAGMA_HANDLER_HPP

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

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/main_shell.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/user_input.hpp>

#include <string>

namespace metashell
{
  namespace iface
  {
    class pragma_handler
    {
    public:
      virtual ~pragma_handler() {}

      virtual std::string arguments() const = 0;
      virtual std::string description() const = 0;

      virtual void run(const data::command::iterator& name_begin_,
                       const data::command::iterator& name_end_,
                       const data::command::iterator& args_begin_,
                       const data::command::iterator& args_end_,
                       main_shell& shell_,
                       displayer& displayer_) const = 0;

      virtual data::code_completion code_complete(data::command::const_iterator,
                                                  data::command::const_iterator,
                                                  main_shell&) const = 0;
    };
  }
}

#endif
