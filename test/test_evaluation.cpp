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

#include "test_shell.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_non_existing_class)
{
  test_shell sh;
  sh.line_available("hello");
  BOOST_CHECK_EQUAL("", sh.info());
  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK(!sh.error().empty());
}

BOOST_AUTO_TEST_CASE(test_accept_empty_input)
{
  test_shell sh;
  sh.line_available("");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_space_input)
{
  test_shell sh;
  sh.line_available(" ");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_tab_input)
{
  test_shell sh;
  sh.line_available("\t");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_vertical_tab_input)
{
  test_shell sh;
  sh.line_available("\v");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_new_line_input)
{
  test_shell sh;
  sh.line_available("\n");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_carrige_return_input)
{
  test_shell sh;
  sh.line_available("\r");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_two_space_input)
{
  test_shell sh;
  sh.line_available("  ");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_macro_in_config)
{
  metashell::config cfg = metashell::config::empty;
  cfg.macros.push_back("FOO=int");
  test_shell sh(cfg, 80);

  sh.line_available("FOO");

  BOOST_CHECK_EQUAL("int", sh.output());
}

namespace
{
  void test_definition_and_query(
    const std::string& definition_,
    const std::string& query_,
    const std::string& expected_result_
  )
  {
    test_shell sh;

    sh.line_available(definition_);
    BOOST_CHECK_EQUAL("", sh.output());
    BOOST_CHECK_EQUAL("", sh.error());

    sh.line_available(query_);
    BOOST_CHECK_EQUAL(expected_result_, sh.output());
  }
}

BOOST_AUTO_TEST_CASE(test_typedef_in_the_middle_of_a_line)
{
  test_definition_and_query("bool typedef * x;", "x", "bool *");
  test_definition_and_query("char typedef * x;", "x", "char *");
  test_definition_and_query("const typedef int x;", "x", "const int");
  test_definition_and_query("double typedef * x;", "x", "double *");
  test_definition_and_query("float typedef * x;", "x", "float *");
  test_definition_and_query("int typedef * x;", "x", "int *");
  test_definition_and_query("long typedef int x;", "x", "long");
  test_definition_and_query("short typedef int x;", "x", "short");
  test_definition_and_query("signed typedef int x;", "x", "int");
  test_definition_and_query("unsigned typedef int x;", "x", "unsigned int");
  test_definition_and_query("void typedef * x;", "x", "void *");
  test_definition_and_query("volatile typedef int x;", "x", "volatile int");
  test_definition_and_query("wchar_t typedef * x;", "x", "wchar_t *");
}

BOOST_AUTO_TEST_CASE(
  test_typedef_in_the_middle_of_a_line_starting_with_an_identifier
)
{
  test_shell sh;

  sh.line_available("struct y;");
  sh.line_available("y typedef * x;");
  BOOST_CHECK_EQUAL("", sh.error());

  sh.line_available("x");
  BOOST_CHECK_EQUAL("y *", sh.output());
}

BOOST_AUTO_TEST_CASE(test_history_is_stored)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("int");

  BOOST_REQUIRE_EQUAL(1, history.size());
  BOOST_CHECK_EQUAL("int", history.front());
}

BOOST_AUTO_TEST_CASE(test_empty_line_is_not_stored_in_history)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("");

  BOOST_CHECK_EQUAL(0, history.size());
}

BOOST_AUTO_TEST_CASE(
  test_line_containing_just_whitespace_is_not_stored_in_history
)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available(" ");

  BOOST_CHECK_EQUAL(0, history.size());
}

BOOST_AUTO_TEST_CASE(
  test_the_same_thing_following_each_other_is_not_added_to_history_twice
)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("int");
  sh.line_available("int");

  BOOST_REQUIRE_EQUAL(1, history.size());
  BOOST_CHECK_EQUAL("int", history.front());
}

BOOST_AUTO_TEST_CASE(test_accept_c_comment_input)
{
  test_shell sh;
  sh.line_available("/* some comment */");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_accept_cpp_comment_input)
{
  test_shell sh;
  sh.line_available("// some comment");

  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_comment_is_stored_in_history)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("// some comment");

  BOOST_CHECK_EQUAL(1, history.size());
}

namespace
{
  void generate_warning(metashell::shell& sh_)
  {
    sh_.line_available("enum color { red, green, blue };");
    sh_.line_available(
      "template <int>"
      "void f(color c)"
      "{"
        "switch (c)"
        "{"
        "case green: break;"
        "}"
      "}"
    );
  }
}

BOOST_AUTO_TEST_CASE(test_warnings)
{
  test_shell sh;

  generate_warning(sh);

  BOOST_CHECK(!sh.error().empty());
}

BOOST_AUTO_TEST_CASE(test_disabled_warnings)
{
  metashell::config cfg = metashell::config::empty;
  cfg.warnings_enabled = false;
  test_shell sh(cfg, 80);

  generate_warning(sh);

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_extra_clang_arg)
{
  metashell::config cfg = metashell::config::empty;
  cfg.extra_clang_args.push_back("-DFOO=double");
  test_shell sh(cfg, 80);

  sh.line_available("FOO");

  BOOST_CHECK_EQUAL("", sh.error());
  BOOST_CHECK_EQUAL("double", sh.output());
}

