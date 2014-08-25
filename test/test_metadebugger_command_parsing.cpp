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
  metadebugger_command_handler_map::command_map_t command_map =
  {
    {"asd", nullptr},
    {"efg", nullptr}
  };

  metadebugger_command_handler_map map(command_map);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "asd");

  JUST_ASSERT_EQUAL(command.get_key(), "asd");
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "efg");

  JUST_ASSERT_EQUAL(command.get_key(), "efg");
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  JUST_ASSERT_EQUAL(command.get_key(), "asd");
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "e");

  JUST_ASSERT_EQUAL(command.get_key(), "efg");
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_2)
{
  metadebugger_command_handler_map::command_map_t command_map =
  {
    {"asd", nullptr},
    {"afg", nullptr}
  };

  metadebugger_command_handler_map map(command_map);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  JUST_ASSERT_EQUAL(command.get_key(), "asd");
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "af");

  JUST_ASSERT_EQUAL(command.get_key(), "afg");
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_3)
{
  metadebugger_command_handler_map::command_map_t command_map =
  {
    {"asd", nullptr},
    {"a", nullptr}
  };

  metadebugger_command_handler_map map(command_map);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  JUST_ASSERT_EQUAL(command.get_key(), "asd");
  JUST_ASSERT_EQUAL(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  JUST_ASSERT_EQUAL(command.get_key(), "a");
  JUST_ASSERT_EQUAL(args, "");
}

JUST_TEST_CASE(test_mdb_command_handler_map_command_selection_4)
{
  metadebugger_command_handler_map::command_map_t command_map =
  {
    {"asd", nullptr},
    {"asf", nullptr}
  };

  metadebugger_command_handler_map map(command_map);

  JUST_ASSERT(!map.get_command_for_line(""));
  JUST_ASSERT(!map.get_command_for_line("a"));
  JUST_ASSERT(!map.get_command_for_line("as"));
}

JUST_TEST_CASE(test_mdb_command_handler_map_argument_passing)
{
  metadebugger_command_handler_map::command_map_t command_map =
  {
    {"asf", nullptr}
  };

  metadebugger_command_handler_map map(command_map);

  metadebugger_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "a abc");

  JUST_ASSERT_EQUAL(command.get_key(), "asf");
  JUST_ASSERT_EQUAL(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "asf   abc");

  JUST_ASSERT_EQUAL(command.get_key(), "asf");
  JUST_ASSERT_EQUAL(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "as   ab c");

  JUST_ASSERT_EQUAL(command.get_key(), "asf");
  JUST_ASSERT_EQUAL(args, "ab c");

  std::tie(command, args) = get_command_from_map(map, "a   ");

  JUST_ASSERT_EQUAL(command.get_key(), "asf");
  JUST_ASSERT_EQUAL(args, "");
}
