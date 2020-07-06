// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command_line_argument_list.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <tuple>

using namespace metashell::data;

namespace
{
  struct expected_removal_result
  {
    std::pair<std::optional<std::string>, command_line_argument_list> values;

    expected_removal_result(std::optional<std::string> warning_,
                            command_line_argument_list args_)
      : values{std::move(warning_), std::move(args_)}
    {
    }
  };

  std::ostream& operator<<(std::ostream& out_,
                           const expected_removal_result& r_)
  {
    out_ << "{";
    if (r_.values.first)
    {
      out_ << "\"" << *r_.values.first << "\"";
    }
    else
    {
      out_ << "<none>";
    }
    return out_ << ", " << r_.values.second << "}";
  }

  bool operator==(const expected_removal_result& lhs_,
                  const std::pair<std::optional<std::string>,
                                  command_line_argument_list>& rhs_)
  {
    return lhs_.values == rhs_;
  }
}

TEST(command_line_argument_list, arch_removal)
{
  ASSERT_EQ(expected_removal_result(std::nullopt, command_line_argument_list{}),
            remove_multiple_arch_arguments(command_line_argument_list{}));

  ASSERT_EQ(expected_removal_result(
                std::nullopt, command_line_argument_list{"foo", "bar", "baz"}),
            remove_multiple_arch_arguments(
                command_line_argument_list{"foo", "bar", "baz"}));

  ASSERT_EQ(
      expected_removal_result(
          std::nullopt, command_line_argument_list{"foo", "bar", "-arch"}),
      remove_multiple_arch_arguments(
          command_line_argument_list{"foo", "bar", "-arch"}));

  ASSERT_EQ(expected_removal_result(
                std::nullopt, command_line_argument_list{"foo", "bar", "-arch",
                                                         "armv7", "baz"}),
            remove_multiple_arch_arguments(command_line_argument_list{
                "foo", "bar", "-arch", "armv7", "baz"}));

  ASSERT_EQ(
      expected_removal_result(
          std::string{"Removed argument -arch armv7s because of earlier "
                      "argument: -arch armv7."},
          command_line_argument_list{"foo", "bar", "-arch", "armv7", "baz"}),
      remove_multiple_arch_arguments(command_line_argument_list{
          "foo", "bar", "-arch", "armv7", "-arch", "armv7s", "baz"}));

  ASSERT_EQ(
      expected_removal_result(
          std::string{"Removed arguments -arch armv7s -arch armv7r because of "
                      "earlier argument: -arch armv7."},
          command_line_argument_list{"foo", "bar", "-arch", "armv7", "baz"}),
      remove_multiple_arch_arguments(
          command_line_argument_list{"foo", "bar", "-arch", "armv7", "-arch",
                                     "armv7s", "baz", "-arch", "armv7r"}));
}
