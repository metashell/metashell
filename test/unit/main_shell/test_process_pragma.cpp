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

#include <metashell/main_shell/process_pragma.hpp>

#include <metashell/iface/pragma_handler.hpp>

#include <metashell/core/command.hpp>
#include <metashell/core/null_displayer.hpp>

#include <metashell/mock/main_shell.hpp>

#include "run.hpp"

#include <gtest/gtest.h>

using namespace metashell;

namespace
{
  class test_handler : public iface::pragma_handler
  {
  public:
    explicit test_handler(bool& run_flag_) : _run_flag(run_flag_) {}

    std::string arguments() const override { return "a|b|c"; }
    std::string description() const override { return "Foo bar"; }

    void run(const data::command::iterator&,
             const data::command::iterator&,
             const data::command::iterator&,
             const data::command::iterator&,
             iface::main_shell&,
             iface::displayer&) const override
    {
      _run_flag = true;
    }

    data::code_completion code_complete(data::command::const_iterator,
                                        data::command::const_iterator,
                                        iface::main_shell&) const override
    {
      return data::code_completion{};
    }

  private:
    bool& _run_flag;
  };
}

TEST(process_pragma, handler_sets_run_flag)
{
  core::null_displayer d;
  bool flag = false;
  run(test_handler(flag), data::cpp_code("foo"), d);

  ASSERT_TRUE(flag);
}

TEST(process_pragma, processing_non_existing_handler)
{
  const data::command cmd =
      core::to_command(data::cpp_code(/* #pragma metashell */ "foo"));

  core::null_displayer d;
  mock::main_shell sh;
  ASSERT_ANY_THROW(
      main_shell::process_pragma({}, cmd.begin(), cmd.end(), sh, d));
}

TEST(process_pragma, processing_existing_handler)
{
  bool foo_run = false;

  std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>> m;
  m.emplace(data::pragma_name{"foo"}, std::make_unique<test_handler>(foo_run));

  const data::command cmd =
      core::to_command(data::cpp_code(/* #pragma metashell */ "foo"));

  core::null_displayer d;
  mock::main_shell sh;
  main_shell::process_pragma(m, cmd.begin(), cmd.end(), sh, d);

  ASSERT_TRUE(foo_run);
}

TEST(process_pragma, pragma_with_two_token_name_is_called)
{
  bool foo_bar_run = false;

  std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>> m;
  m.emplace(data::pragma_name{"foo", "bar"},
            std::make_unique<test_handler>(foo_bar_run));

  const data::command cmd =
      core::to_command(data::cpp_code(/* #pragma metashell */ "foo bar"));

  core::null_displayer d;
  mock::main_shell sh;
  main_shell::process_pragma(m, cmd.begin(), cmd.end(), sh, d);

  ASSERT_TRUE(foo_bar_run);
}

TEST(process_pragma,
     pragma_with_two_token_name_is_called_when_prefix_is_available)
{
  bool foo_bar_run = false;
  bool foo_run = false;

  std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>> m;
  m.emplace(data::pragma_name{"foo"}, std::make_unique<test_handler>(foo_run));
  m.emplace(data::pragma_name{"foo", "bar"},
            std::make_unique<test_handler>(foo_bar_run));

  const data::command cmd =
      core::to_command(data::cpp_code(/* #pragma metashell */ "foo bar"));

  core::null_displayer d;
  mock::main_shell sh;
  main_shell::process_pragma(m, cmd.begin(), cmd.end(), sh, d);

  ASSERT_FALSE(foo_run);
  ASSERT_TRUE(foo_bar_run);
}

TEST(process_pragma, pragma_prefix_is_selected_when_longer_version_is_available)
{
  bool foo_bar_run = false;
  bool foo_run = false;

  std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>> m;
  m.emplace(data::pragma_name{"foo"}, std::make_unique<test_handler>(foo_run));
  m.emplace(data::pragma_name{"foo", "bar"},
            std::make_unique<test_handler>(foo_bar_run));

  const data::command cmd =
      core::to_command(data::cpp_code(/* #pragma metashell */ "foo x"));

  core::null_displayer d;
  mock::main_shell sh;
  main_shell::process_pragma(m, cmd.begin(), cmd.end(), sh, d);

  ASSERT_TRUE(foo_run);
  ASSERT_FALSE(foo_bar_run);
}
