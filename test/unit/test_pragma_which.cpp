// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command.hpp>

#include <metashell/core/engine_constant.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/pragma_which.hpp>
#include <metashell/core/shell.hpp>

#include "test_config.hpp"

#include <gtest/gtest.h>

using namespace metashell;

// Using a macro to get useful source location information for failed
// assertions.
#ifdef CHECK_IF_DISPLAYS_ERROR
#error CHECK_IF_DISPLAYS_ERROR already defined
#endif
#define CHECK_IF_DISPLAYS_ERROR(command_, error_message_)                      \
  {                                                                            \
    core::in_memory_displayer d;                                               \
    core::shell sh(test_config(), "", "", "", core::create_failing_engine());  \
    sh.line_available((command_), d);                                          \
                                                                               \
    ASSERT_EQ(std::vector<std::string>{(error_message_)}, d.errors());         \
  }

namespace
{
  core::pragma_which::parsed_arguments
  parse_arguments(const std::string& arguments_)
  {
    const data::command arguments{data::cpp_code(arguments_)};
    return core::pragma_which::parse_arguments(
        "which", arguments.begin(), arguments.end());
  }

  core::pragma_which::parsed_arguments sys(const boost::filesystem::path& path_,
                                           bool all_)
  {
    return {data::include_argument(data::include_type::sys, path_), all_};
  }

  core::pragma_which::parsed_arguments
  quote(const boost::filesystem::path& path_, bool all_)
  {
    return {data::include_argument(data::include_type::quote, path_), all_};
  }

  void test_invalid_arguments(const std::string& prefix_)
  {
    CHECK_IF_DISPLAYS_ERROR(prefix_, "Error: No header is provided.")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + " hello",
                            "Error: Argument of which is not a header to "
                            "include. Did you mean <hello> or \"hello\"?")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + " <hello", "Error: closing > is missing.")
    CHECK_IF_DISPLAYS_ERROR(
        prefix_ + " \"hello", "Error: closing \" is missing.")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + " <hello> \"world\"",
                            "Error: More than one arguments provided.");
    CHECK_IF_DISPLAYS_ERROR(prefix_ + " \"hello\" <world>",
                            "Error: More than one arguments provided.");
    CHECK_IF_DISPLAYS_ERROR(prefix_ + " -", "Error: Invalid argument: -")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + " -asd", "Error: Invalid argument: -asd")
  }

  template <bool All>
  void test_valid_arguments()
  {
    const std::string prefix = All ? "-all " : "";

    ASSERT_EQ(sys("hello", All), parse_arguments(prefix + "<hello>"));
    ASSERT_EQ(sys("hello/world.hpp", All),
              parse_arguments(prefix + "<hello/world.hpp>"));
    ASSERT_EQ(sys("hello\\world.hpp", All),
              parse_arguments(prefix + "<hello\\world.hpp>"));
    ASSERT_EQ(quote("hello", All), parse_arguments(prefix + "\"hello\""));
    ASSERT_EQ(quote("hello/world.hpp", All),
              parse_arguments(prefix + "\"hello/world.hpp\""));
    ASSERT_EQ(quote("hello\\world.hpp", All),
              parse_arguments(prefix + "\"hello\\world.hpp\""));
  }
}

TEST(pragma_which, invalid_arguments) { test_invalid_arguments("#msh which"); }

TEST(pragma_which, all_invalid_arguments)
{
  test_invalid_arguments("#msh which -all");
}

TEST(pragma_which, path_parsing) { test_valid_arguments<false>(); }

TEST(pragma_which, path_parsing_with_all) { test_valid_arguments<true>(); }
