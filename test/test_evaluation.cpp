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


