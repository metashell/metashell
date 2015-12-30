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

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_hide_definition_error)
{
  const auto r = run_metashell(
    {
      command("#msh show cpp_errors off"),
      command("void f() { return 13; }")
    }
  );

  auto i = r.begin() + 3;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_hide_evaluation_error)
{
  const auto r = run_metashell(
    {
      command("#msh show cpp_errors off"),
      command("void int")
    }
  );

  auto i = r.begin() + 3;

  JUST_ASSERT(i == r.end());
}

