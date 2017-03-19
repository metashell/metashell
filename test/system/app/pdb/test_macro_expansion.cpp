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
#include <metashell/system_test/prompt.hpp>

#include <gtest/gtest.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <stdexcept>

using namespace metashell::system_test;
using pattern::_;

TEST(pdb, object_like_macro)
{
  metashell_instance mi;
  mi.command("#define FOO bar");
  mi.command("#msh pdb FOO");

  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("FOO")), 0, 1},
      {frame(type("FOO"), _, _, event_kind::macro_expansion), 1, 1},
        {frame(type("bar"), _, _, event_kind::rescanning), 2, 1},
          {frame(type("bar"), _, _, event_kind::expanded_code), 3, 0}
    }),
    mi.command("ft").front()
  );

  // clang-format on
}

TEST(pdb, function_like_macro)
{
  metashell_instance mi;
  mi.command("#define FOO(x) x world");
  mi.command("#msh pdb FOO(hello)");

  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("FOO(hello)")), 0, 1},
      {frame(type("FOO(hello)"), _, _, event_kind::macro_expansion), 1, 1},
        {frame(type("hello world"), _, _, event_kind::rescanning), 2, 1},
          {frame(type("hello world"), _, _, event_kind::expanded_code), 3, 0}
    }),
    mi.command("ft").front()
  );

  // clang-format on
}

TEST(pdb, recursive_macro_call)
{
  metashell_instance mi;
  mi.command("#define FOO(x) x world");
  mi.command("#define BAR(x, y) FOO(x) FOO(y)");
  mi.command("#msh pdb BAR(hello,macro)");

  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("BAR(hello,macro)")), 0, 1},
      {frame(type("BAR(hello,macro)"), _, _, event_kind::macro_expansion), 1, 1},
        {frame(type("FOO(hello) FOO(macro)"), _, _, event_kind::rescanning), 2, 3},
          {frame(type("FOO(hello)"), _, _, event_kind::macro_expansion), 3, 1},
            {frame(type("hello world"), _, _, event_kind::rescanning), 4, 1},
              {frame(type("hello world"), _, _, event_kind::expanded_code), 5, 0},
          {frame(type("FOO(macro)"), _, _, event_kind::macro_expansion), 3, 1},
            {frame(type("macro world"), _, _, event_kind::rescanning), 4, 1},
              {frame(type("macro world"), _, _, event_kind::expanded_code), 5, 0},
          {frame(type("hello world macro world"), _, _, event_kind::expanded_code), 3, 0}
    }),
    mi.command("ft").front()
  );

  // clang-format on
}
