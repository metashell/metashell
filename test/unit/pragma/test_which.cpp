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

#include <metashell/pragma/which.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/exception.hpp>

#include <metashell/core/command.hpp>
#include <metashell/core/in_memory_displayer.hpp>

#include <metashell/mock/main_shell.hpp>

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
    mock::main_shell sh;                                                       \
    const auto cmd = core::to_command(data::cpp_code("which"));                \
    const auto carg = core::to_command(data::cpp_code(command_));              \
                                                                               \
    try                                                                        \
    {                                                                          \
      pragma::which().run(                                                     \
          cmd.begin(), cmd.end(), carg.begin(), carg.end(), sh, d);            \
      FAIL() << "Expected error.";                                             \
    }                                                                          \
    catch (const data::exception& err)                                         \
    {                                                                          \
      ASSERT_EQ(std::string(error_message_), err.what());                      \
    }                                                                          \
  }

namespace
{
  pragma::which::parsed_arguments parse_arguments(const std::string& arguments_)
  {
    const data::command arguments =
        core::to_command(data::cpp_code(arguments_));
    return pragma::which::parse_arguments(
        "which", arguments.begin(), arguments.end());
  }

  pragma::which::parsed_arguments sys(const boost::filesystem::path& path_,
                                      bool all_)
  {
    return {data::include_argument(data::include_type::sys, path_), all_};
  }

  pragma::which::parsed_arguments quote(const boost::filesystem::path& path_,
                                        bool all_)
  {
    return {data::include_argument(data::include_type::quote, path_), all_};
  }

  void test_invalid_arguments(const std::string& prefix_)
  {
    CHECK_IF_DISPLAYS_ERROR(prefix_, "No header is provided.")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + "hello",
                            "Argument of which is not a header to include. Did "
                            "you mean <hello> or \"hello\"?")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + "<hello", "closing > is missing.")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + "\"hello", "closing \" is missing.")
    CHECK_IF_DISPLAYS_ERROR(
        prefix_ + "<hello> \"world\"", "More than one arguments provided.");
    CHECK_IF_DISPLAYS_ERROR(
        prefix_ + "\"hello\" <world>", "More than one arguments provided.");
    CHECK_IF_DISPLAYS_ERROR(prefix_ + "-", "Invalid argument: -")
    CHECK_IF_DISPLAYS_ERROR(prefix_ + "-asd", "Invalid argument: -asd")
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
} // namespace

TEST(which, invalid_arguments) { test_invalid_arguments(""); }

TEST(which, all_invalid_arguments) { test_invalid_arguments("-all "); }

TEST(which, path_parsing) { test_valid_arguments<false>(); }

TEST(which, path_parsing_with_all) { test_valid_arguments<true>(); }
