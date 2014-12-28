// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/mdb_command_handler_map.hpp>

#include <just/test.hpp>

#include "util.hpp"

using namespace metashell;

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_1)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{{"asd"}}, non_repeatable, nullptr, "", "", ""},
    {{{"efg"}}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, {"asd"});

  JUST_ASSERT_EQUAL_CONTAINER({"asd"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, {"efg"});

  JUST_ASSERT_EQUAL_CONTAINER({"efg"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  JUST_ASSERT_EQUAL_CONTAINER({"asd"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "e");

  JUST_ASSERT_EQUAL_CONTAINER({"efg"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_2)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"asd"}, non_repeatable, nullptr, "", "", ""},
    {{"afg"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  JUST_ASSERT_EQUAL_CONTAINER({"asd"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "af");

  JUST_ASSERT_EQUAL_CONTAINER({"afg"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_3)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"asd"}, non_repeatable, nullptr, "", "", ""},
    {{"a"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  JUST_ASSERT_EQUAL_CONTAINER({"asd"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  JUST_ASSERT_EQUAL_CONTAINER({"a"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_4)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"asd"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  JUST_ASSERT(!map.get_command_for_line(""));
  JUST_ASSERT(!map.get_command_for_line("a"));
  JUST_ASSERT(!map.get_command_for_line("as"));
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_5)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"asd", "xyz"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "asd");

  JUST_ASSERT_EQUAL_CONTAINER({"asd", "xyz"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "xyz");

  JUST_ASSERT_EQUAL_CONTAINER({"asd", "xyz"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "asf");

  JUST_ASSERT_EQUAL_CONTAINER({"asf"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_6)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"ft", "forwardtrace"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "f");

  JUST_ASSERT_EQUAL_CONTAINER({"ft", "forwardtrace"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_7)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"ft", "forwardtrace", "fff"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "f");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"ft", "forwardtrace", "fff"},
    command.get_keys()
  );
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_8)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"ft", "forwardtrace"}, non_repeatable, nullptr, "", "", ""},
    {{"fff"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  JUST_ASSERT(!map.get_command_for_line("f"));
}

JUST_TEST_CASE(test_mdb_command_handler_map_argument_passing)
{
  mdb_command_handler_map::commands_t commands =
  {
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "a abc");

  JUST_ASSERT_EQUAL_CONTAINER({"asf"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "asf   abc");

  JUST_ASSERT_EQUAL_CONTAINER({"asf"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "as   ab c");

  JUST_ASSERT_EQUAL_CONTAINER({"asf"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "ab c");

  std::tie(command, args) = get_command_from_map(map, "a   ");

  JUST_ASSERT_EQUAL_CONTAINER({"asf"}, command.get_keys());
  JUST_ASSERT_EQUAL(args, "");
}
