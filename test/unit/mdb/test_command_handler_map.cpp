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

#include <metashell/mdb/command_handler_map.hpp>

#include <gtest/gtest.h>

#include "util.hpp"

using namespace metashell;

TEST(command_handler_map, command_selection_1)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{{name_type("asd")}},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{{name_type("efg")}},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> asd = data::mdb_command::parse("asd");
  ASSERT_TRUE(asd);
  ASSERT_EQ(std::vector<name_type>{name_type("asd")},
            get_command_from_map(map, *asd).get_keys());
  ASSERT_TRUE(asd->arguments().empty());

  const std::optional<data::mdb_command> efg = data::mdb_command::parse("efg");
  ASSERT_TRUE(efg);
  ASSERT_EQ(std::vector<name_type>{name_type("efg")},
            get_command_from_map(map, *efg).get_keys());
  ASSERT_TRUE(efg->arguments().empty());

  const std::optional<data::mdb_command> a = data::mdb_command::parse("a");
  ASSERT_TRUE(a);
  ASSERT_EQ(std::vector<name_type>{name_type("asd")},
            get_command_from_map(map, *a).get_keys());
  ASSERT_TRUE(a->arguments().empty());

  const std::optional<data::mdb_command> e = data::mdb_command::parse("e");
  ASSERT_TRUE(e);
  ASSERT_EQ(std::vector<name_type>{name_type("efg")},
            get_command_from_map(map, *e).get_keys());
  ASSERT_TRUE(e->arguments().empty());
}

TEST(command_handler_map, command_selection_2)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("asd")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("afg")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> as = data::mdb_command::parse("as");
  ASSERT_TRUE(as);
  ASSERT_EQ(std::vector<name_type>{name_type("asd")},
            get_command_from_map(map, *as).get_keys());
  ASSERT_TRUE(as->arguments().empty());

  const std::optional<data::mdb_command> af = data::mdb_command::parse("af");
  ASSERT_TRUE(af);
  ASSERT_EQ(std::vector<name_type>{name_type("afg")},
            get_command_from_map(map, *af).get_keys());
  ASSERT_TRUE(af->arguments().empty());
}

TEST(command_handler_map, command_selection_3)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("asd")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("a")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> as = data::mdb_command::parse("as");
  ASSERT_TRUE(as);
  ASSERT_EQ(std::vector<name_type>{name_type("asd")},
            get_command_from_map(map, *as).get_keys());
  ASSERT_TRUE(as->arguments().empty());

  const std::optional<data::mdb_command> a = data::mdb_command::parse("a");
  ASSERT_TRUE(a);
  ASSERT_EQ(std::vector<name_type>{name_type("a")},
            get_command_from_map(map, *a).get_keys());
  ASSERT_TRUE(a->arguments().empty());
}

TEST(command_handler_map, command_selection_4)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("asd")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("asf")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  ASSERT_FALSE(map.get_command(data::mdb_command::name_type("a")));
  ASSERT_FALSE(map.get_command(data::mdb_command::name_type("as")));
}

TEST(command_handler_map, command_selection_5)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("asd"), name_type("xyz")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("asf")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> asd = data::mdb_command::parse("asd");
  ASSERT_TRUE(asd);
  ASSERT_EQ((std::vector<name_type>{name_type("asd"), name_type("xyz")}),
            get_command_from_map(map, *asd).get_keys());
  ASSERT_TRUE(asd->arguments().empty());

  const std::optional<data::mdb_command> xyz = data::mdb_command::parse("xyz");
  ASSERT_TRUE(xyz);
  ASSERT_EQ((std::vector<name_type>{name_type("asd"), name_type("xyz")}),
            get_command_from_map(map, *xyz).get_keys());
  ASSERT_TRUE(xyz->arguments().empty());

  const std::optional<data::mdb_command> asf = data::mdb_command::parse("asf");
  ASSERT_TRUE(asf);
  ASSERT_EQ(std::vector<name_type>{name_type("asf")},
            get_command_from_map(map, *asf).get_keys());
  ASSERT_TRUE(asf->arguments().empty());
}

TEST(command_handler_map, command_selection_6)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("ft"), name_type("forwardtrace")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("asf")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> f = data::mdb_command::parse("f");
  ASSERT_TRUE(f);
  ASSERT_EQ(
      (std::vector<name_type>{name_type("ft"), name_type("forwardtrace")}),
      get_command_from_map(map, *f).get_keys());
  ASSERT_TRUE(f->arguments().empty());
}

TEST(command_handler_map, command_selection_7)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("ft"), name_type("forwardtrace"), name_type("fff")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("asf")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> f = data::mdb_command::parse("f");
  ASSERT_TRUE(f);
  ASSERT_EQ((std::vector<name_type>{
                name_type("ft"), name_type("forwardtrace"), name_type("fff")}),
            get_command_from_map(map, *f).get_keys());
  ASSERT_TRUE(f->arguments().empty());
}

TEST(command_handler_map, command_selection_8)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("ft"), name_type("forwardtrace")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""},
      {{name_type("fff")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  ASSERT_FALSE(map.get_command(data::mdb_command::name_type("f")));
}

TEST(command_handler_map, argument_passing)
{
  using name_type = data::mdb_command::name_type;

  mdb::command_handler_map::commands_t commands = {
      {{name_type("asf")},
       mdb::repeatable_t::non_repeatable,
       nullptr,
       "",
       "",
       ""}};

  mdb::command_handler_map map(commands);

  const std::optional<data::mdb_command> a = data::mdb_command::parse("a abc");
  ASSERT_TRUE(a);
  ASSERT_EQ(std::vector<name_type>{name_type("asf")},
            get_command_from_map(map, *a).get_keys());
  {
    auto i = a->arguments().begin();
    const auto e = a->arguments().end();
    ASSERT_TRUE(i != e);
    ASSERT_EQ(*i, "abc");
    ++i;
    ASSERT_TRUE(i == e);
  }

  const std::optional<data::mdb_command> asf =
      data::mdb_command::parse("asf   abc");
  ASSERT_TRUE(asf);
  ASSERT_EQ(std::vector<name_type>{name_type("asf")},
            get_command_from_map(map, *asf).get_keys());
  {
    auto i = asf->arguments().begin();
    const auto e = asf->arguments().end();
    ASSERT_TRUE(i != e);
    ASSERT_EQ(*i, "abc");
    ++i;
    ASSERT_TRUE(i == e);
  }

  const std::optional<data::mdb_command> as =
      data::mdb_command::parse("as   ab c");
  ASSERT_TRUE(as);
  ASSERT_EQ(std::vector<name_type>{name_type("asf")},
            get_command_from_map(map, *as).get_keys());
  {
    auto i = as->arguments().begin();
    const auto e = as->arguments().end();
    ASSERT_TRUE(i != e);
    ASSERT_EQ(*i, "ab");
    ++i;
    ASSERT_TRUE(i != e);
    ASSERT_EQ(*i, "c");
    ++i;
    ASSERT_TRUE(i == e);
  }

  const std::optional<data::mdb_command> a2 = data::mdb_command::parse("a   ");
  ASSERT_TRUE(a2);
  ASSERT_EQ(std::vector<name_type>{name_type("asf")},
            get_command_from_map(map, *a2).get_keys());
  ASSERT_TRUE(a2->arguments().empty());
}
