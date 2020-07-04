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

#include <metashell/pragma/pp.hpp>

namespace metashell
{
  namespace pragma
  {
    std::string pp::arguments() const { return "<exp>"; }

    std::string pp::description() const
    {
      return "Displays the preprocessed expression.";
    }

    void pp::run(const data::command::iterator&,
                 const data::command::iterator&,
                 const data::command::iterator& args_begin_,
                 const data::command::iterator& args_end_,
                 iface::main_shell& shell_,
                 iface::displayer& displayer_) const
    {
      shell_.preprocess(
          displayer_, tokens_to_string(args_begin_, args_end_), false);
    }

    data::code_completion
    pp::code_complete(data::command::const_iterator begin_,
                      data::command::const_iterator end_,
                      iface::main_shell& shell_) const
    {
      return shell_.code_complete(
          data::user_input{data::join_tokens(begin_, end_)}, false);
    }
  }
}
