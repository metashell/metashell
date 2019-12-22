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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <pattern/regex.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::regex;

TEST(macros, getting_defined_macro)
{
  metashell_instance mi;
  mi.command("#define FOO bar");

  if (!using_msvc())
  {
    ASSERT_EQ(
        cpp_code(regex("#define FOO bar")), mi.command("#msh macros").front());
  }
}

TEST(macros, getting_defined_macro_name)
{
  metashell_instance mi;
  mi.command("#define FOO bar");

  if (!using_msvc())
  {
    const json_string macro_names = mi.command("#msh macro names").front();
    ASSERT_EQ(cpp_code(regex("FOO")), macro_names);
    ASSERT_NE(cpp_code(regex("#define")), macro_names);
  }
}

TEST(macros, defined_from_cli)
{
  const std::string define = using_msvc() ? "/D" : "-D";
  metashell_instance mi({"--", define + "FOO=double"});
  mi.command("#msh preprocessor mode");
  ASSERT_EQ(cpp_code("double"), mi.command("FOO").front());
}
