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

#include <metashell/metadebugger_command_handler_map.hpp>

#include <just/test.hpp>

#include "util.hpp"

using namespace metashell;

std::tuple<metadebugger_command, std::string> get_command_from_map(
    const metadebugger_command_handler_map& map,
    const std::string& line)
{
  auto opt_pair = map.get_command_for_line(line);

  JUST_ASSERT(opt_pair);

  return *opt_pair;
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_1)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{{"asd"}}, non_repeatable, nullptr, "", "", ""},
    {{{"efg"}}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, {"asd"});

  JUST_ASSERT(equal(command.get_keys(), {"asd"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, {"efg"});

  JUST_ASSERT(equal(command.get_keys(), {"efg"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  JUST_ASSERT(equal(command.get_keys(), {"asd"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "e");

  JUST_ASSERT(equal(command.get_keys(), {"efg"}));
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_2)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"asd"}, non_repeatable, nullptr, "", "", ""},
    {{"afg"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  JUST_ASSERT(equal(command.get_keys(), {"asd"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "af");

  JUST_ASSERT(equal(command.get_keys(), {"afg"}));
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_3)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"asd"}, non_repeatable, nullptr, "", "", ""},
    {{"a"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  JUST_ASSERT(equal(command.get_keys(), {"asd"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  JUST_ASSERT(equal(command.get_keys(), {"a"}));
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_4)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"asd"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  JUST_ASSERT(!map.get_command_for_line(""));
  JUST_ASSERT(!map.get_command_for_line("a"));
  JUST_ASSERT(!map.get_command_for_line("as"));
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_5)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"asd", "xyz"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "asd");

  JUST_ASSERT(equal(command.get_keys(), {"asd", "xyz"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "xyz");

  JUST_ASSERT(equal(command.get_keys(), {"asd", "xyz"}));
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "asf");

  JUST_ASSERT(equal(command.get_keys(), {"asf"}));
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_6)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"ft", "forwardtrace"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "f");

  JUST_ASSERT(equal(command.get_keys(), {"ft", "forwardtrace"}));
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_7)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"ft", "forwardtrace", "fff"}, non_repeatable, nullptr, "", "", ""},
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "f");

  JUST_ASSERT(equal(command.get_keys(), {"ft", "forwardtrace", "fff"}));
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_8)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"ft", "forwardtrace"}, non_repeatable, nullptr, "", "", ""},
    {{"fff"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  JUST_ASSERT(!map.get_command_for_line("f"));
}

JUST_TEST_CASE(test_mdb_command_handler_map_argument_passing)
{
  metadebugger_command_handler_map::commands_t commands =
  {
    {{"asf"}, non_repeatable, nullptr, "", "", ""}
  };

  metadebugger_command_handler_map map(commands);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "a abc");

  JUST_ASSERT(equal(command.get_keys(), {"asf"}));
  JUST_ASSERT_EQUAL(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "asf   abc");

  JUST_ASSERT(equal(command.get_keys(), {"asf"}));
  JUST_ASSERT_EQUAL(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "as   ab c");

  JUST_ASSERT(equal(command.get_keys(), {"asf"}));
  JUST_ASSERT_EQUAL(args, "ab c");

  std::tie(command, args) = get_command_from_map(map, "a   ");

  JUST_ASSERT(equal(command.get_keys(), {"asf"}));
  JUST_ASSERT_EQUAL(args, "");
}
