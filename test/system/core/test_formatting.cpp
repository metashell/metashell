// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/any_of.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/path_builder.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(formatting, basic)
{
  metashell_instance mi;
  mi.command(
      "namespace metashell"
      "{"
      "template <>"
      "struct format<int>"
      "{"
      "typedef double type;"
      "};"
      "}");

  ASSERT_EQ(type("double"), mi.command("int").front());
}

TEST(formatting, tag_dispatched)
{
  metashell_instance mi;
  mi.command(
      "struct foo_tag {};"

      "struct foo"
      "{"
      "typedef foo_tag tag;"
      "};"

      "namespace metashell"
      "{"
      "template <>"
      "struct format_impl<foo_tag>"
      "{"
      "typedef format_impl type;"

      "template <class T>"
      "struct apply"
      "{"
      "typedef char type;"
      "};"
      "};"
      "}");

  ASSERT_EQ(type("char"), mi.command("foo").front());
}

TEST(formatting, disabled)
{
  metashell_instance mi;
  mi.command("template <class... Ts> struct template_with_a_long_name {};");

  ASSERT_EQ(type("template_with_a_long_name<int, double, char>"),
            mi.command("template_with_a_long_name<int, double, char>").front());
}

TEST(formatting, nested_mpl_vector)
{
  const std::string vector_hpp =
      path_builder() / "metashell" / "formatter" / "vector.hpp";

  metashell_instance mi;
  mi.command("#include <" + vector_hpp + ">");

  ASSERT_EQ(any_of<type>("boost_::mpl::vector<boost_::mpl::vector<int>>",
                         "boost_::mpl::vector<boost_::mpl::vector<int> >"),
            mi.command("boost::mpl::vector<boost::mpl::vector<int>>").front());
}
