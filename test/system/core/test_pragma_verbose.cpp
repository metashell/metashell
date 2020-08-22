// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/code_completer.hpp>
#include <metashell/system_test/code_completion_result.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(pragma_verbose, code_complete)
{
  auto res = code_completion_result::create;
  code_completer c;

  ASSERT_EQ(c("#msh verbose"), res(" on", " off", " 1", " 0"));
  ASSERT_EQ(c("#msh verbose "), res("on", "off", "1", "0"));
  ASSERT_EQ(c("#msh verbose 1"), res(""));
  ASSERT_EQ(c("#msh verbose 0"), res(""));
  ASSERT_EQ(c("#msh verbose o"), res("n", "ff"));
  ASSERT_EQ(c("#msh verbose on"), res(""));
  ASSERT_EQ(c("#msh verbose of"), res("f"));
  ASSERT_EQ(c("#msh verbose off"), res(""));
}
