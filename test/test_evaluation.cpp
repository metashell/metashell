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
#include "breaking_environment.hpp"

#include <metashell/metashell.hpp>
#include <metashell/path_builder.hpp>

#include <just/test.hpp>

JUST_TEST_CASE(test_non_existing_class)
{
  test_shell sh;
  sh.line_available("hello");
  JUST_ASSERT_EQUAL("", sh.info());
  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT(!sh.error().empty());
}

JUST_TEST_CASE(test_accept_empty_input)
{
  test_shell sh;
  sh.line_available("");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_space_input)
{
  test_shell sh;
  sh.line_available(" ");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_tab_input)
{
  test_shell sh;
  sh.line_available("\t");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_vertical_tab_input)
{
  test_shell sh;
  sh.line_available("\v");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_new_line_input)
{
  test_shell sh;
  sh.line_available("\n");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_carrige_return_input)
{
  test_shell sh;
  sh.line_available("\r");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_two_space_input)
{
  test_shell sh;
  sh.line_available("  ");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_macro_in_config)
{
  metashell::config cfg;
  cfg.macros.push_back("FOO=int");
  test_shell sh(cfg, 80);

  sh.line_available("FOO");

  JUST_ASSERT_EQUAL("int", sh.output());
}

namespace
{
  void test_definition_and_query(
    const std::string& init_line_,
    const std::string& definition_,
    const std::string& query_,
    const std::string& expected_result_
  )
  {
    test_shell sh;

    sh.line_available(init_line_);
    sh.line_available(definition_);
    JUST_ASSERT_EQUAL("", sh.output());
    JUST_ASSERT_EQUAL("", sh.error());

    sh.line_available(query_);
    JUST_ASSERT_EQUAL(expected_result_, sh.output());
  }

  void test_definition_and_query(
    const std::string& definition_,
    const std::string& query_,
    const std::string& expected_result_
  )
  {
    test_definition_and_query("", definition_, query_, expected_result_);
  }
}

JUST_TEST_CASE(test_typedef_in_the_middle_of_a_line)
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

#ifndef METASHELL_DISABLE_CONSTEXPR
JUST_TEST_CASE(test_defining_constexpr_function)
{
  const std::string scalar_hpp =
    metashell::path_builder() / "metashell" / "scalar.hpp";

  test_definition_and_query(
    "#include <" + scalar_hpp + ">",
    "constexpr int f() { return 13; }",
    "SCALAR(f())",
    "std::integral_constant<int, 13>"
  );
}
#endif

JUST_TEST_CASE(
  test_typedef_in_the_middle_of_a_line_starting_with_an_identifier
)
{
  test_shell sh;

  sh.line_available("struct y;");
  sh.line_available("y typedef * x;");
  JUST_ASSERT_EQUAL("", sh.error());

  sh.line_available("x");
  JUST_ASSERT_EQUAL("y *", sh.output());
}

JUST_TEST_CASE(test_history_is_stored)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("int");

  JUST_ASSERT_EQUAL(1u, history.size());
  JUST_ASSERT_EQUAL("int", history.front());
}

JUST_TEST_CASE(test_empty_line_is_not_stored_in_history)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("");

  JUST_ASSERT_EQUAL(0u, history.size());
}

JUST_TEST_CASE(
  test_line_containing_just_whitespace_is_not_stored_in_history
)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available(" ");

  JUST_ASSERT_EQUAL(0u, history.size());
}

JUST_TEST_CASE(
  test_the_same_thing_following_each_other_is_not_added_to_history_twice
)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("int");
  sh.line_available("int");

  JUST_ASSERT_EQUAL(1u, history.size());
  JUST_ASSERT_EQUAL("int", history.front());
}

JUST_TEST_CASE(test_accept_c_comment_input)
{
  test_shell sh;
  sh.line_available("/* some comment */");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_accept_cpp_comment_input)
{
  test_shell sh;
  sh.line_available("// some comment");

  JUST_ASSERT_EQUAL("", sh.output());
  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_comment_is_stored_in_history)
{
  std::vector<std::string> history;
  test_shell sh(history);

  sh.line_available("// some comment");

  JUST_ASSERT_EQUAL(1u, history.size());
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

JUST_TEST_CASE(test_warnings)
{
  test_shell sh;

  generate_warning(sh);

  JUST_ASSERT(!sh.error().empty());
}

JUST_TEST_CASE(test_disabled_warnings)
{
  metashell::config cfg;
  cfg.warnings_enabled = false;
  test_shell sh(cfg, 80);

  generate_warning(sh);

  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_extra_clang_arg)
{
  metashell::config cfg;
  cfg.extra_clang_args.push_back("-DFOO=double");
  test_shell sh(cfg, 80);

  sh.line_available("FOO");

  JUST_ASSERT_EQUAL("", sh.error());
  JUST_ASSERT_EQUAL("double", sh.output());
}

JUST_TEST_CASE(test_throwing_environment_update_not_breaking_shell)
{
  metashell::config cfg;
  breaking_environment* e = new breaking_environment(cfg);
  test_shell sh(cfg, e);
  e->append_throw_from_now();

  sh.store_in_buffer("typedef int foo;");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_throwing_environment_not_breaking_validate)
{
  metashell::config cfg;
  breaking_environment e(cfg);
  e.get_appended_throw_from_now();
  const metashell::result
    r = metashell::validate_code("typedef int foo;", cfg, e, "<input>");

  JUST_ASSERT(!r.errors.empty());
}

JUST_TEST_CASE(test_variable_definition)
{
  using metashell::command;
  JUST_ASSERT(metashell::is_environment_setup_command(command("int x;")));
}

JUST_TEST_CASE(test_function_definition)
{
  using metashell::command;
  JUST_ASSERT(metashell::is_environment_setup_command(command("void f() {}")));
}

JUST_TEST_CASE(test_multiline_input)
{
  test_shell sh;
  sh.line_available("const \\");
  sh.line_available("int");

  JUST_ASSERT_EQUAL("", sh.error());
  JUST_ASSERT_EQUAL("const int", sh.output());
}

JUST_TEST_CASE(test_three_line_input)
{
  test_shell sh;
  sh.line_available("const \\");
  sh.line_available("int \\");
  sh.line_available("*");

  JUST_ASSERT_EQUAL("", sh.error());
  JUST_ASSERT_EQUAL("const int *", sh.output());
}

JUST_TEST_CASE(test_prompt_is_different_in_multiline_input)
{
  test_shell sh;
  sh.line_available("const \\");

  JUST_ASSERT_EQUAL("...> ", sh.prompt());
}

