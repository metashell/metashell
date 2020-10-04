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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/path_builder.hpp>

#include <gtest/gtest.h>

#include <cassert>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  std::string appended_since(const std::string& old_, const std::string& new_)
  {
    assert(new_.size() >= old_.size());
    assert(new_.substr(0, old_.size()) == old_);
    return new_.substr(old_.size());
  }

  std::string current_env(metashell_instance& mi_)
  {
    return *cpp_code(mi_.command("#pragma metashell environment").front())
                .code()
                .value();
  }
} // namespace

TEST(shell_environment, extending_with_pragma_warns)
{
  metashell_instance mi;

  ASSERT_EQ(
      (std::vector<json_string>{
          to_json_string(comment(
              "You don't need the environment add pragma to add this to the"
              " environment. The following command does this as well:")),
          to_json_string(cpp_code("typedef int x;")),
          to_json_string(prompt(">"))}),
      mi.command("#pragma metashell environment add typedef int x;"));
}

TEST(shell_environment, resetting)
{
  metashell_instance mi;
  mi.command("typedef int foo;");
  mi.command("#pragma metashell environment reset");

  ASSERT_EQ(error(_), mi.command("foo").front());
}

TEST(shell_environment, resetting_does_not_remove_built_in_macros)
{
  const std::string scalar_hpp = path_builder() / "metashell" / "scalar.hpp";

  metashell_instance mi;
  mi.command("#pragma metashell environment reset");
  mi.command("#include <" + scalar_hpp + ">");

  ASSERT_EQ(type(_), mi.command("SCALAR(__METASHELL_MAJOR)").front());
}

TEST(shell_environment, restoring_after_reset_from_environment_stack)
{
  metashell_instance mi;
  mi.command("typedef int foo;");

  ASSERT_EQ(comment("Environment stack has 1 entry"),
            mi.command("#pragma metashell environment push").front());

  mi.command("#pragma metashell environment reset");

  ASSERT_EQ(comment("Environment stack is empty"),
            mi.command("#pragma metashell environment pop").front());

  ASSERT_EQ(type("int"), mi.command("foo").front());
}

TEST(shell_environment,
     add_invalid_code_does_not_change_environment_and_displays_error)
{
  const cpp_code breaking("typedef nonexisting_type x;");

  metashell_instance mi;

  const std::vector<json_string> original_env =
      mi.command("#pragma metashell environment");

  const std::vector<json_string> br = mi.command(
      "#pragma metashell environment add " + *breaking.code().value());
  ASSERT_EQ(error(_), br[0]);
  ASSERT_EQ(comment(_), br[1]);
  ASSERT_EQ(breaking, br[2]);

  ASSERT_EQ(original_env, mi.command("#pragma metashell environment"));
}

TEST(shell_environment, appended_since_when_nothing_appended)
{
  ASSERT_EQ("", appended_since("", ""));
}

TEST(shell_environment, appended_since_when_something_appended)
{
  ASSERT_EQ(" world", appended_since("hello", "hello world"));
}

TEST(shell_environment, extending_environment_with_pragma)
{
  metashell_instance mi;

  const std::string original_env = current_env(mi);

  mi.command("#pragma metashell environment add typedef int x;");

  ASSERT_EQ("typedef int x;\n", appended_since(original_env, current_env(mi)));
}
