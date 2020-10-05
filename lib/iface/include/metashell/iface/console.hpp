#ifndef METASHELL_IFACE_CONSOLE_HPP
#define METASHELL_IFACE_CONSOLE_HPP

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

#include <metashell/data/colored_string.hpp>

namespace metashell
{
  namespace iface
  {
    class console
    {
    public:
      enum class user_answer
      {
        next_page,
        show_all,
        quit
      };

      virtual ~console() = default;

      virtual void show(const data::colored_string& s_) = 0;
      virtual void new_line() = 0;

      virtual user_answer ask_for_continuation() = 0;

      virtual int width() const = 0;
      virtual int height() const = 0;
    };
  } // namespace iface
} // namespace metashell

#endif
