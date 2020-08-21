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

#include <metashell/pragma/echo.hpp>

namespace metashell
{
  namespace pragma
  {
    std::string echo::arguments() const { return "<text>"; }

    std::string echo::description() const
    {
      return "Display a comment containing <text>.";
    }

    void echo::run(const data::command::iterator&,
                   const data::command::iterator&,
                   const data::command::iterator& args_begin_,
                   const data::command::iterator& args_end_,
                   iface::main_shell&,
                   iface::displayer& displayer_) const
    {
      displayer_.show_comment(
          data::text(data::tokens_to_string(args_begin_, args_end_).value()));
    }

    data::code_completion
    echo::code_complete(data::command::const_iterator begin_,
                        data::command::const_iterator end_,
                        iface::main_shell&) const
    {
      return begin_ == end_ ? data::code_completion{data::user_input{}} :
                              data::code_completion{};
    }
  }
}
