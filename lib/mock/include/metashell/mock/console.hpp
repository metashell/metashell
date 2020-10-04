#ifndef METASHELL_MOCK_CONSOLE_HPP
#define METASHELL_MOCK_CONSOLE_HPP

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

#include <metashell/iface/console.hpp>

#include <gmock/gmock.h>

namespace metashell
{
  namespace mock
  {
    class console : public iface::console
    {
    public:
      MOCK_METHOD1(show, void(const data::colored_string&));
      MOCK_METHOD0(new_line, void());

      MOCK_METHOD0(ask_for_continuation, user_answer());

      MOCK_CONST_METHOD0(width, int());
      MOCK_CONST_METHOD0(height, int());
    };
  } // namespace mock
} // namespace metashell

#endif
