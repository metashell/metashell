#ifndef METASHELL_PAGER_HPP
#define METASHELL_PAGER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/iface/console.hpp>

namespace metashell
{

  class pager
  {
  public:
    pager(iface::console& console);

    void show(const data::colored_string& string);
    bool new_line();

  private:
    iface::console& console_;

    bool show_all = false;

    int lines_in_current_page = 0;
    int chars_in_current_line = 0;
  };

} // namespace metashell

#endif
