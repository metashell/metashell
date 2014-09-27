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

#include "test_shell.hpp"
#include "argv0.hpp"

#include <just/test.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  bool newline_char(char c_) { return c_ == '\n' || c_ == '\r'; }

  std::string first_line(const std::string& s_)
  {
    using std::string;
    using std::find_if;

    return string(s_.begin(), find_if(s_.begin(), s_.end(), newline_char));
  }
}

JUST_TEST_CASE(test_pragma_evaluate_runs_a_metaprogram)
{
  test_shell sh;
  sh.line_available("typedef int x;");
  sh.line_available("#pragma metashell evaluate x");

  JUST_ASSERT_EQUAL("int", first_line(sh.output()));
}

JUST_TEST_CASE(test_pragma_evaluate_displays_error_for_invalid_code)
{
  test_shell sh;
  sh.line_available(
    "#pragma metashell evaluate nonexisting_type"
  );

  JUST_ASSERT(!sh.error().empty());
}

JUST_TEST_CASE(test_pragma_evaluate_warns)
{
  test_shell sh(metashell::empty_config(argv0::get()), 1024);
  sh.line_available("#pragma metashell evaluate int");

  JUST_ASSERT_EQUAL(
    "int\n"
    "// You don't need the evaluate add pragma to evaluate this metaprogram."
    " The following command does this as well:\n"
    "int\n",
    sh.output()
  );
}

