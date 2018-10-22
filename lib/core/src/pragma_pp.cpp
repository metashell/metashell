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

#include <metashell/core/pragma_pp.hpp>

namespace metashell
{
  namespace core
  {
    pragma_pp::pragma_pp(shell& shell_) : _shell(shell_) {}

    std::string pragma_pp::arguments() const { return "<exp>"; }

    std::string pragma_pp::description() const
    {
      return "Displays the preprocessed expression.";
    }

    void pragma_pp::run(const data::command::iterator&,
                        const data::command::iterator&,
                        const data::command::iterator& args_begin_,
                        const data::command::iterator& args_end_,
                        iface::displayer& displayer_) const
    {
      _shell.preprocess(
          displayer_, tokens_to_string(args_begin_, args_end_), false);
    }
  }
}
