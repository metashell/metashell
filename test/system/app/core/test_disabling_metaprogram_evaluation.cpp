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

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

JUST_TEST_CASE(test_disable_metaprogram_evaluation)
{
  metashell_instance mi;
  mi.command("#msh metaprogram evaluation off");
  JUST_ASSERT_EQUAL_CONTAINER({to_json_string(prompt(">"))}, mi.command("int"));
}

JUST_TEST_CASE(
    test_no_errors_for_invalid_metaprograms_when_evaluation_is_disabled)
{
  metashell_instance mi;
  mi.command("#msh metaprogram evaluation off");
  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(prompt(">"))}, mi.command("void int"));
}
