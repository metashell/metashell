// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <gtest/gtest.h>

#include <metashell/core/type.hpp>

#include <metashell/data/type.hpp>

using namespace metashell;

TEST(is_template_type, primitive_types)
{
  ASSERT_FALSE(core::is_template_type(data::type("int")));
  ASSERT_FALSE(core::is_template_type(data::type("void")));
  ASSERT_FALSE(core::is_template_type(data::type("float")));
  ASSERT_FALSE(core::is_template_type(data::type("unsigned long long")));
  ASSERT_FALSE(core::is_template_type(data::type("const double")));
}

TEST(is_template_type, array_type_of_non_template_types)
{
  ASSERT_FALSE(core::is_template_type(data::type("int[3]")));
  ASSERT_FALSE(core::is_template_type(data::type("int&[3]")));
  ASSERT_FALSE(core::is_template_type(data::type("char[0]")));
  ASSERT_FALSE(core::is_template_type(data::type("char *[1]")));
  ASSERT_FALSE(core::is_template_type(data::type("foo[100]")));
}

TEST(is_template_type, pointer_to_non_template_types)
{
  ASSERT_FALSE(core::is_template_type(data::type("int*")));
  ASSERT_FALSE(core::is_template_type(data::type("char *[1]")));
  ASSERT_FALSE(core::is_template_type(data::type("foo ***")));
}

TEST(is_template_type, templates)
{
  ASSERT_TRUE(core::is_template_type(data::type("std::vector<int>")));
  ASSERT_TRUE(core::is_template_type(data::type("foo::bar::foobar<x>")));
  ASSERT_TRUE(core::is_template_type(data::type("a<b<c, d> > >")));
}

TEST(is_template_type, char_literals)
{
  ASSERT_TRUE(core::is_template_type(data::type("foo<'<'>")));
  ASSERT_TRUE(core::is_template_type(data::type("foo<'>'>")));
  ASSERT_TRUE(core::is_template_type(data::type("foo<'<','>'>")));
}
