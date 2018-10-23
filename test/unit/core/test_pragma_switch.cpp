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

#include <metashell/core/command.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/metashell_pragma.hpp>
#include <metashell/core/pragma_switch.hpp>

#include <metashell/mock/shell.hpp>

#include "run.hpp"

#include <gtest/gtest.h>

using namespace metashell;
using ::testing::Return;
using ::testing::NiceMock;

namespace
{
  template <bool ExpectedResult>
  void test_callback_is_called(const std::string& arg_)
  {
    constexpr data::shell_flag flag = data::shell_flag::echo;

    NiceMock<mock::shell> sh;

    ON_CALL(sh, enabled(flag)).WillByDefault(Return(!ExpectedResult));
    EXPECT_CALL(sh, enabled(flag, ExpectedResult));

    core::in_memory_displayer d;
    const core::pragma_switch p(flag);

    const metashell::data::command cmd =
        metashell::core::to_command(data::cpp_code(arg_));
    p.run(cmd.begin(), cmd.begin(), cmd.begin(),
          metashell::core::end_of_pragma_argument_list(cmd.begin(), cmd.end()),
          sh, d);
  }
}

TEST(pragma_switch, calls_updating_callback)
{
  test_callback_is_called<true>("on");
  test_callback_is_called<true>("1");

  test_callback_is_called<false>("off");
  test_callback_is_called<false>("0");
}

TEST(pragma_switch, displays_error_when_extra_arguments_are_given)
{
  core::in_memory_displayer d;

  const core::pragma_switch p(data::shell_flag::echo);
  run(p, data::cpp_code("on foo"), d);

  ASSERT_FALSE(d.errors().empty());
}
