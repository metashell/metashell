// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;
using pattern::_;

TEST(pdb, generating_unprocessed_output)
{
  metashell_instance mi;
  mi.command("#msh pdb hello");

  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("hello")), 0, 2},
      {frame(type("hello"), _, _, event_kind::generated_token), 1, 0},
      {frame(type("\\n"), _, _, event_kind::generated_token), 1, 0}
    }),
    mi.command("ft").front()
  );

  // clang-format on
}
