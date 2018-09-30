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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/path_builder.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(expression_instantiation, good_expression)
{
  const std::string ie_hpp =
      path_builder() / "metashell" / "instantiate_expression.hpp";

  metashell_instance mi;
  mi.command("#include <" + ie_hpp + ">");
  mi.command("template <class T> T mod2(T t_) { return t_ % 2; }");

  ASSERT_EQ(type("metashell::expression_instantiated<true>"),
            mi.command("METASHELL_INSTANTIATE_EXPRESSION( mod2(21) )").front());
}

TEST(expression_instantiation, expression_that_generates_an_error)
{
  const std::string ie_hpp =
      path_builder() / "metashell" / "instantiate_expression.hpp";

  metashell_instance mi;
  mi.command("#include <" + ie_hpp + ">");
  mi.command("#include <string>");
  mi.command("template <class T> T mod2(T t_) { return t_ % 2; }");

  ASSERT_EQ(
      error(_),
      mi.command("METASHELL_INSTANTIATE_EXPRESSION( mod2(std::string()) )")
          .front());
}
