// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/in_memory_displayer.hpp>
#include <metashell/shell.hpp>
#include "test_config.hpp"
#include "argv0.hpp"

#include <just/test.hpp>

#include <algorithm>

using namespace metashell;

JUST_TEST_CASE(test_pragma_evaluate_runs_a_metaprogram)
{
  in_memory_displayer d;
  shell sh(test_config(), d);
  sh.line_available("typedef int x;");
  sh.line_available("#pragma metashell evaluate x");

  JUST_ASSERT(!d.types().empty());
  JUST_ASSERT_EQUAL(type("int"), d.types().front());
}

JUST_TEST_CASE(test_pragma_evaluate_displays_error_for_invalid_code)
{
  in_memory_displayer d;
  shell sh(test_config(), d);
  sh.line_available("#pragma metashell evaluate nonexisting_type");

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_pragma_evaluate_warns)
{
  in_memory_displayer d;
  shell sh(metashell::empty_config(argv0::get()), d);
  sh.line_available("#pragma metashell evaluate int");

  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      text{
        paragraph(
          "You don't need the evaluate add pragma to evaluate this metaprogram."
          " The following command does this as well:"
        ),
        paragraph("int")
      }
    },
    d.comments()
  );
}

