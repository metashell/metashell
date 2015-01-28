// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/metashell.hpp>

using namespace metashell;

JUST_TEST_CASE(test_repair_string_type_1) {
  JUST_ASSERT_EQUAL("bool", repair_type_string("_Bool"));
}

JUST_TEST_CASE(test_repair_string_type_2) {
  JUST_ASSERT_EQUAL("<bool", repair_type_string("<_Bool"));
}

JUST_TEST_CASE(test_repair_string_type_3) {
  JUST_ASSERT_EQUAL("bool>", repair_type_string("_Bool>"));
}

JUST_TEST_CASE(test_repair_string_type_4) {
  JUST_ASSERT_EQUAL(" bool>", repair_type_string(" _Bool>"));
}

JUST_TEST_CASE(test_repair_string_type_5) {
  JUST_ASSERT_EQUAL("a_Bool", repair_type_string("a_Bool"));
}

JUST_TEST_CASE(test_repair_string_type_6) {
  JUST_ASSERT_EQUAL("_Boola", repair_type_string("_Boola"));
}

JUST_TEST_CASE(test_repair_string_type_7) {
  JUST_ASSERT_EQUAL("b_Boola", repair_type_string("b_Boola"));
}
