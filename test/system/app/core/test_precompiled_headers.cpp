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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(precompiled_headers, enabling_and_disabling)
{
  const std::vector<json_string> is_on{
      to_json_string(comment({paragraph("precompiled header usage is on")})),
      to_json_string(prompt(">"))};
  const std::vector<json_string> is_off{
      to_json_string(comment({paragraph("precompiled header usage is off")})),
      to_json_string(prompt(">"))};

  metashell_instance mi;

  ASSERT_EQ(is_on, mi.command("#msh precompiled_headers"));
  ASSERT_EQ(is_off, mi.command("#msh precompiled_headers off"));
  ASSERT_EQ(is_off, mi.command("#msh precompiled_headers"));
  ASSERT_EQ(is_on, mi.command("#msh precompiled_headers on"));
  ASSERT_EQ(is_on, mi.command("#msh precompiled_headers"));
  ASSERT_EQ(is_on, mi.command("#msh precompiled_headers on"));
  ASSERT_EQ(is_on, mi.command("#msh precompiled_headers"));
}
