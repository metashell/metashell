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
#include <metashell/system_test/type.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  bool macro_definition_works(const std::string& macro_)
  {
    metashell_instance mi;

    mi.command("#msh pdb #define " + macro_);

    // clang-format off

    return
      call_graph({
        {frame(type("#define " + macro_)), 0, 2},
        {frame(type(macro_), _, _, event_kind::macro_definition), 1, 0},
        {frame(type("\\n"), _, _, event_kind::skipped_token), 1, 0}
      })
      == mi.command("ft").front();

    // clang-format on
  }

  bool macro_deletion_works(const std::string& name_,
                            const std::string& args_and_body_)
  {
    metashell_instance mi;

    mi.command("#define " + name_ + args_and_body_);
    mi.command("#msh pdb #undef " + name_);

    // clang-format off

    return
      call_graph({
        {frame(type("#undef " + name_)), 0, 2},
        {frame(type(name_), _, _, event_kind::macro_deletion), 1, 0},
        {frame(type("\\n"), _, _, event_kind::skipped_token), 1, 0}
      })
      == mi.command("ft").front();

    // clang-format on
  }
}

TEST(pdb, test_macro_definition)
{
  ASSERT_TRUE(macro_definition_works("FOO bar"));
  ASSERT_TRUE(macro_definition_works("FOO() bar"));
  ASSERT_TRUE(macro_definition_works("FOO(x) bar"));
  ASSERT_TRUE(macro_definition_works("FOO(x, y) bar"));
  ASSERT_TRUE(macro_definition_works("FOO(x, y, ...) bar"));
}

TEST(pdb, test_macro_deletion)
{
  ASSERT_TRUE(macro_deletion_works("FOO", " bar"));
  ASSERT_TRUE(macro_deletion_works("FOO", "() bar"));
  ASSERT_TRUE(macro_deletion_works("FOO", "(x) bar"));
  ASSERT_TRUE(macro_deletion_works("FOO", "(x, y) bar"));
  ASSERT_TRUE(macro_deletion_works("FOO", "(x, y, ...) bar"));
}
