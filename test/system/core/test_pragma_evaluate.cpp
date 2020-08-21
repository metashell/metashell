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

#include <metashell/system_test/code_completer.hpp>
#include <metashell/system_test/code_completion_result.hpp>
#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(pragma_evaluate, runs_a_metaprogram)
{
  metashell_instance mi;
  mi.command("typedef int x;");

  ASSERT_EQ(
      (std::vector<json_string>{
          to_json_string(type("int")),
          to_json_string(comment{
              paragraph("You don't need the evaluate add pragma to evaluate "
                        "this metaprogram."
                        " The following command does this as well:"),
              paragraph("x")}),
          to_json_string(prompt(">"))}),
      mi.command("#pragma metashell evaluate x"));
}

TEST(pragma_evaluate, displays_error_for_invalid_code)
{
  metashell_instance mi;

  const std::vector<json_string> re =
      mi.command("#pragma metashell evaluate nonexisting_type");
  ASSERT_EQ(error(_), re[0]);
  ASSERT_EQ(
      (comment{paragraph("You don't need the evaluate add pragma to evaluate "
                         "this metaprogram."
                         " The following command does this as well:"),
               paragraph("nonexisting_type")}),
      re[1]);
}

TEST(pragma_evaluate, warns)
{
  metashell_instance mi;

  ASSERT_EQ(
      (std::vector<json_string>{
          to_json_string(type("int")),
          to_json_string(comment{
              paragraph("You don't need the evaluate add pragma to evaluate "
                        "this metaprogram."
                        " The following command does this as well:"),
              paragraph("int")}),
          to_json_string(prompt(">"))}),
      mi.command("#pragma metashell evaluate int"));
}

TEST(pragma_evaluate, code_complete)
{
  // most of the code completion testing is covered in test_completion.cpp

  code_completer c;

  ASSERT_EQ(c("#msh evaluate #ms"), code_completion_result{});
  ASSERT_EQ(c("#msh evaluate #pragma metash"), code_completion_result{});
}
