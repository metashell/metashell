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

#include <metashell/pragma_without_arguments.hpp>
#include <metashell/shell.hpp>

using namespace metashell;

std::string pragma_without_arguments::arguments() const { return ""; }

void pragma_without_arguments::run(const data::command::iterator& name_begin_,
                                   const data::command::iterator& name_end_,
                                   const data::command::iterator& args_begin_,
                                   const data::command::iterator& args_end_,
                                   iface::displayer& displayer_) const
{
  if (args_begin_ == args_end_)
  {
    run(displayer_);
  }
  else
  {
    displayer_.show_error(
        "Invalid arguments for #msh " +
        data::tokens_to_string(name_begin_, name_end_).value() + ": " +
        tokens_to_string(args_begin_, args_end_).value());
  }
}
