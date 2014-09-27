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

#include <just/test.hpp>

#include <metashell/is_template_type.hpp>

using namespace metashell;

JUST_TEST_CASE(test_is_template_type_primitive_types) {
  JUST_ASSERT(!is_template_type("int"));
  JUST_ASSERT(!is_template_type("void"));
  JUST_ASSERT(!is_template_type("float"));
  JUST_ASSERT(!is_template_type("unsigned long long"));
  JUST_ASSERT(!is_template_type("const double"));
}

JUST_TEST_CASE(test_is_template_type_array_type_of_non_template_types) {
  JUST_ASSERT(!is_template_type("int[3]"));
  JUST_ASSERT(!is_template_type("int&[3]"));
  JUST_ASSERT(!is_template_type("char[0]"));
  JUST_ASSERT(!is_template_type("char *[1]"));
  JUST_ASSERT(!is_template_type("foo[100]"));
}

JUST_TEST_CASE(test_is_template_type_pointer_to_non_template_types) {
  JUST_ASSERT(!is_template_type("int*"));
  JUST_ASSERT(!is_template_type("char *[1]"));
  JUST_ASSERT(!is_template_type("foo ***"));
}

JUST_TEST_CASE(test_is_template_type_templates) {
  JUST_ASSERT(is_template_type("std::vector<int>"));
  JUST_ASSERT(is_template_type("foo::bar::foobar<x>"));
  JUST_ASSERT(is_template_type("a<b<c, d> > >"));
}

JUST_TEST_CASE(test_is_template_type_char_literals) {
  JUST_ASSERT(is_template_type("foo<'<'>"));
  JUST_ASSERT(is_template_type("foo<'>'>"));
  JUST_ASSERT(is_template_type("foo<'<','>'>"));
}
