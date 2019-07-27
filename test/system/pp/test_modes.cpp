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
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(modes, preprocessor_mode)
{
  metashell_instance mi;
  mi.command("#msh preprocessor mode");

  ASSERT_EQ(comment("display preprocessed commands is on"),
            mi.command("#msh preprocessed echo").front());
  ASSERT_EQ(comment("display C++ errors is off"),
            mi.command("#msh show cpp_errors").front());
  ASSERT_EQ(comment("evaluation of metaprograms is off"),
            mi.command("#msh metaprogram evaluation").front());
}

TEST(modes, metaprogram_mode)
{
  metashell_instance mi;
  mi.command("#msh metaprogram mode");

  ASSERT_EQ(comment("display preprocessed commands is off"),
            mi.command("#msh preprocessed echo").front());
  ASSERT_EQ(comment("display C++ errors is on"),
            mi.command("#msh show cpp_errors").front());
  ASSERT_EQ(comment("evaluation of metaprograms is on"),
            mi.command("#msh metaprogram evaluation").front());
}

TEST(modes, undef_in_preprocessor_mode)
{
  metashell_instance mi;
  mi.command("#msh preprocessor mode");
  mi.command("#define FOO");

  ASSERT_EQ(prompt(">"), mi.command("#undef FOO").front());
}
