#ifndef METASHELL_PRAGMA_MACRO_HPP
#define METASHELL_PRAGMA_MACRO_HPP

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

#include <metashell/pragma/without_arguments.hpp>

#include <metashell/data/user_input.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace pragma
  {
    class macro : public without_arguments
    {
    public:
      macro(std::string description_, std::vector<data::user_input> commands_);

      std::string description() const override;

      void run(iface::main_shell&, iface::displayer&) const override;

    private:
      std::vector<data::user_input> _commands;
      std::string _description;
    };
  } // namespace pragma
} // namespace metashell

#endif
