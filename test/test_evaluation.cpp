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

#include "test_config.hpp"
#include "breaking_environment.hpp"
#include "util.hpp"

#include <metashell/shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/path_builder.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/in_memory_history.hpp>

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_non_existing_class)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("hello", d);
  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_accept_empty_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_space_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available(" ", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_tab_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("\t", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_vertical_tab_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("\v", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_new_line_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("\n", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_carrige_return_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("\r", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_two_space_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("  ", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_macro_in_config)
{
  metashell::config cfg;
  cfg.max_template_depth = 20;
  cfg.macros.push_back("FOO=int");
  in_memory_displayer d;
  shell sh(cfg);

  sh.line_available("FOO", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
}

namespace
{
  void test_definition_and_query(
    const std::string& init_line_,
    const std::string& definition_,
    const std::string& query_,
    const type& expected_result_
  )
  {
    in_memory_displayer d;
    shell sh(test_config());

    sh.line_available(init_line_, d);
    sh.line_available(definition_, d);
    JUST_ASSERT_EMPTY_CONTAINER(d.types());
    JUST_ASSERT_EMPTY_CONTAINER(d.errors());

    sh.line_available(query_, d);
    JUST_ASSERT_EMPTY_CONTAINER(d.errors());
    JUST_ASSERT_EQUAL_CONTAINER({expected_result_}, d.types());
  }

  void test_definition_and_query(
    const std::string& definition_,
    const std::string& query_,
    const type& expected_result_
  )
  {
    test_definition_and_query("", definition_, query_, expected_result_);
  }
}

JUST_TEST_CASE(test_typedef_in_the_middle_of_a_line)
{
  test_definition_and_query("bool typedef * x;", "x", type("bool *"));
  test_definition_and_query("char typedef * x;", "x", type("char *"));
  test_definition_and_query("const typedef int x;", "x", type("const int"));
  test_definition_and_query("double typedef * x;", "x", type("double *"));
  test_definition_and_query("float typedef * x;", "x", type("float *"));
  test_definition_and_query("int typedef * x;", "x", type("int *"));
  test_definition_and_query("long typedef int x;", "x", type("long"));
  test_definition_and_query("short typedef int x;", "x", type("short"));
  test_definition_and_query("signed typedef int x;", "x", type("int"));
  test_definition_and_query(
    "unsigned typedef int x;",
    "x",
    type("unsigned int")
  );
  test_definition_and_query("void typedef * x;", "x", type("void *"));
  test_definition_and_query(
    "volatile typedef int x;",
    "x",
    type("volatile int")
  );
  test_definition_and_query("wchar_t typedef * x;", "x", type("wchar_t *"));
}

JUST_TEST_CASE(test_defining_constexpr_function)
{
  const std::string scalar_hpp =
    metashell::path_builder() / "metashell" / "scalar.hpp";

  in_memory_displayer d;
  shell sh(test_config());

  sh.line_available("#include <" + scalar_hpp + ">", d);
  sh.line_available("constexpr int f() { return 13; }", d);
  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());

  sh.line_available("SCALAR(f())", d);
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL(1u, d.types().size());
  JUST_ASSERT(d.types().front().is_integral_constant(type("int"), "13"));
}

JUST_TEST_CASE(
  test_typedef_in_the_middle_of_a_line_starting_with_an_identifier
)
{
  in_memory_displayer d;
  shell sh(test_config());

  sh.line_available("struct y;", d);
  sh.line_available("y typedef * x;", d);
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());

  sh.line_available("x", d);
  JUST_ASSERT_EQUAL_CONTAINER({type("y *")}, d.types());
}

JUST_TEST_CASE(test_history_is_stored)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config());
  sh.history(h);

  sh.line_available("int", d);

  JUST_ASSERT_EQUAL_CONTAINER({"int"}, h.commands());
}

JUST_TEST_CASE(test_empty_line_is_not_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config());
  sh.history(h);

  sh.line_available("", d);

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());
}

JUST_TEST_CASE(
  test_line_containing_just_whitespace_is_not_stored_in_history
)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config());
  sh.history(h);

  sh.line_available(" ", d);

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());
}

JUST_TEST_CASE(
  test_the_same_thing_following_each_other_is_not_added_to_history_twice
)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config());
  sh.history(h);

  sh.line_available("int", d);
  sh.line_available("int", d);

  JUST_ASSERT_EQUAL_CONTAINER({"int"}, h.commands());
}

JUST_TEST_CASE(test_accept_c_comment_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("/* some comment */", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_cpp_comment_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("// some comment", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_comment_is_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config());
  sh.history(h);

  sh.line_available("// some comment", d);

  JUST_ASSERT_EQUAL(1u, h.commands().size());
}

namespace
{
  void generate_warning(metashell::shell& sh_, iface::displayer& displayer_)
  {
    sh_.line_available("#warning hello", displayer_);
  }
}

JUST_TEST_CASE(test_warnings)
{
  in_memory_displayer d;
  shell sh(test_config());

  generate_warning(sh, d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_disabled_warnings)
{
  metashell::config cfg;
  cfg.warnings_enabled = false;
  in_memory_displayer d;
  shell sh(cfg);

  generate_warning(sh, d);

  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_extra_clang_arg)
{
  metashell::config cfg;
  cfg.extra_clang_args.push_back("-DFOO=double");
  cfg.max_template_depth = 20;
  in_memory_displayer d;
  shell sh(cfg);

  sh.line_available("FOO", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER({type("double")}, d.types());
}

JUST_TEST_CASE(test_throwing_environment_update_not_breaking_shell)
{
  metashell::config cfg;
  breaking_environment* e = new breaking_environment(cfg);
  in_memory_displayer d;
  command_processor_queue cpq;
  shell sh(cfg, std::unique_ptr<breaking_environment>(e), cpq);
  e->append_throw_from_now();

  sh.store_in_buffer("typedef int foo;", d);

  JUST_ASSERT(!d.errors().empty());
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

JUST_TEST_CASE(test_is_environment_setup_with_leading_whitespace)
{
  using metashell::command;
  JUST_ASSERT(!metashell::is_environment_setup_command(command(" int")));
}

JUST_TEST_CASE(test_is_environment_setup_without_leading_whitespace)
{
  using metashell::command;
  JUST_ASSERT(!metashell::is_environment_setup_command(command("int")));
}

JUST_TEST_CASE(test_multiline_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("const \\", d);
  sh.line_available("int", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER({type("const int")}, d.types());
}

JUST_TEST_CASE(test_three_line_input)
{
  in_memory_displayer d;
  shell sh(test_config());
  sh.line_available("const \\", d);
  sh.line_available("int \\", d);
  sh.line_available("*", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER({type("const int *")}, d.types());
}

JUST_TEST_CASE(test_prompt_is_different_in_multiline_input)
{
  null_displayer d;
  shell sh(test_config());
  sh.line_available("const \\", d);

  JUST_ASSERT_EQUAL("...> ", sh.prompt());
}

