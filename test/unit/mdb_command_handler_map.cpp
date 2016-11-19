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

#include <gtest/gtest.h>

#include "util.hpp"

using namespace metashell;

TEST(mdb_command_handler_map, command_selection_1)
{
  mdb_command_handler_map::commands_t commands = {
      {{{"asd"}}, repeatable_t::non_repeatable, nullptr, "", "", ""},
      {{{"efg"}}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, {"asd"});

  ASSERT_EQ(std::vector<std::string>{"asd"}, command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, {"efg"});

  ASSERT_EQ(std::vector<std::string>{"efg"}, command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  ASSERT_EQ(std::vector<std::string>{"asd"}, command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, "e");

  ASSERT_EQ(std::vector<std::string>{"efg"}, command.get_keys());
  ASSERT_EQ(args, "");
}

TEST(mdb_command_handler_map, command_selection_2)
{
  mdb_command_handler_map::commands_t commands = {
      {{"asd"}, repeatable_t::non_repeatable, nullptr, "", "", ""},
      {{"afg"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  ASSERT_EQ(std::vector<std::string>{"asd"}, command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, "af");

  ASSERT_EQ(std::vector<std::string>{"afg"}, command.get_keys());
  ASSERT_EQ(args, "");
}

TEST(mdb_command_handler_map, command_selection_3)
{
  mdb_command_handler_map::commands_t commands = {
      {{"asd"}, repeatable_t::non_repeatable, nullptr, "", "", ""},
      {{"a"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "as");

  ASSERT_EQ(std::vector<std::string>{"asd"}, command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, "a");

  ASSERT_EQ(std::vector<std::string>{"a"}, command.get_keys());
  ASSERT_EQ(args, "");
}

TEST(mdb_command_handler_map, command_selection_4)
{
  mdb_command_handler_map::commands_t commands = {
      {{"asd"}, repeatable_t::non_repeatable, nullptr, "", "", ""},
      {{"asf"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  ASSERT_FALSE(map.get_command_for_line(""));
  ASSERT_FALSE(map.get_command_for_line("a"));
  ASSERT_FALSE(map.get_command_for_line("as"));
}

TEST(mdb_command_handler_map, command_selection_5)
{
  mdb_command_handler_map::commands_t commands = {
      {{"asd", "xyz"}, repeatable_t::non_repeatable, nullptr, "", "", ""},
      {{"asf"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "asd");

  ASSERT_EQ((std::vector<std::string>{"asd", "xyz"}), command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, "xyz");

  ASSERT_EQ((std::vector<std::string>{"asd", "xyz"}), command.get_keys());
  ASSERT_EQ(args, "");

  std::tie(command, args) = get_command_from_map(map, "asf");

  ASSERT_EQ(std::vector<std::string>{"asf"}, command.get_keys());
  ASSERT_EQ(args, "");
}

TEST(mdb_command_handler_map, command_selection_6)
{
  mdb_command_handler_map::commands_t commands = {
      {{"ft", "forwardtrace"},
       repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{"asf"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "f");

  ASSERT_EQ(
      (std::vector<std::string>{"ft", "forwardtrace"}), command.get_keys());
  ASSERT_EQ(args, "");
}

TEST(mdb_command_handler_map, command_selection_7)
{
  mdb_command_handler_map::commands_t commands = {
      {{"ft", "forwardtrace", "fff"},
       repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{"asf"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "f");

  ASSERT_EQ((std::vector<std::string>{"ft", "forwardtrace", "fff"}),
            command.get_keys());
  ASSERT_EQ(args, "");
}

TEST(mdb_command_handler_map, command_selection_8)
{
  mdb_command_handler_map::commands_t commands = {
      {{"ft", "forwardtrace"},
       repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{"fff"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  ASSERT_FALSE(map.get_command_for_line("f"));
}

TEST(mdb_command_handler_map, argument_passing)
{
  mdb_command_handler_map::commands_t commands = {
      {{"asf"}, repeatable_t::non_repeatable, nullptr, "", "", ""}};

  mdb_command_handler_map map(commands);

  mdb_command command;
  std::string args;

  std::tie(command, args) = get_command_from_map(map, "a abc");

  ASSERT_EQ(std::vector<std::string>{"asf"}, command.get_keys());
  ASSERT_EQ(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "asf   abc");

  ASSERT_EQ(std::vector<std::string>{"asf"}, command.get_keys());
  ASSERT_EQ(args, "abc");

  std::tie(command, args) = get_command_from_map(map, "as   ab c");

  ASSERT_EQ(std::vector<std::string>{"asf"}, command.get_keys());
  ASSERT_EQ(args, "ab c");

  std::tie(command, args) = get_command_from_map(map, "a   ");

  ASSERT_EQ(std::vector<std::string>{"asf"}, command.get_keys());
  ASSERT_EQ(args, "");
}
