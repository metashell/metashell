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

#include <metashell/iface/command_processor.hpp>

#include <metashell/core/pragma_without_arguments.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace core
  {
    class pragma_macro : public pragma_without_arguments
    {
    public:
      pragma_macro(std::string description_,
                   std::vector<std::string> commands_,
                   iface::command_processor& shell_);

      virtual std::string description() const override;

      virtual void run(iface::displayer& displayer_) const override;

    private:
      iface::command_processor& _shell;
      std::vector<std::string> _commands;
      std::string _description;
    };
  }
}

#endif
