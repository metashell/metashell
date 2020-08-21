#ifndef METASHELL_IFACE_MAIN_SHELL_HPP
#define METASHELL_IFACE_MAIN_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/shell.hpp>

#include <vector>

namespace metashell
{
  namespace iface
  {
    class main_shell : public shell
    {
    public:
      virtual void switch_to(const data::real_engine_name&) = 0;

      virtual const std::vector<data::real_engine_name>&
      available_engines() const = 0;
    };
  }
}

#endif
