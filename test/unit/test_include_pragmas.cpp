// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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
#include <metashell/engine_constant.hpp>

#include "test_config.hpp"

#include <just/test.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  template <data::include_type Type>
  void test_pragma_includes_displays_includes()
  {
    in_memory_displayer d;
    metashell::shell sh(
        metashell::test_config(), "", "", "",
        metashell::create_engine_with_include_path(Type, {"foo", "bar"}));
    sh.line_available("#msh " + to_string(Type) + "includes", d);

    JUST_ASSERT_EQUAL_CONTAINER({data::text("foo\nbar")}, d.comments());
  }
}

JUST_TEST_CASE(test_pragma_sysincludes_displays_sysincludes)
{
  test_pragma_includes_displays_includes<data::include_type::sys>();
}

JUST_TEST_CASE(test_pragma_quoteincludes_displays_quoteincludes)
{
  test_pragma_includes_displays_includes<data::include_type::quote>();
}
