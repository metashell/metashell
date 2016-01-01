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

#include <metashell/pragma_echo.hpp>

using namespace metashell;

iface::pragma_handler* pragma_echo::clone() const { return new pragma_echo(); }

std::string pragma_echo::arguments() const { return "<text>"; }

std::string pragma_echo::description() const
{
  return "Display a comment containing <text>.";
}

void pragma_echo::run(const data::command::iterator&,
                      const data::command::iterator&,
                      const data::command::iterator& args_begin_,
                      const data::command::iterator& args_end_,
                      iface::displayer& displayer_) const
{
  displayer_.show_comment(
      data::text(data::tokens_to_string(args_begin_, args_end_)));
}
