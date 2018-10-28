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

#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/in_memory_history.hpp>
#include <metashell/core/null_displayer.hpp>

#include <metashell/data/empty_container.hpp>

#include <gtest/gtest.h>

#include "test_shell.hpp"

using namespace metashell;

TEST(mdb_shell, is_stopped_false_by_default)
{
  test_shell sh;

  ASSERT_FALSE(sh.stopped());
}

TEST(mdb_shell, empty_lines)
{
  core::in_memory_displayer d;
  core::in_memory_history h;
  test_shell sh;

  ASSERT_EQ(data::empty_container(), h.commands());

  sh.line_available("", d, h);
  ASSERT_EQ(data::empty_container(), h.commands());
  ASSERT_EQ(data::empty_container(), d.raw_texts());
  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_TRUE(d.call_graphs().empty());

  sh.line_available(" ", d, h);
  ASSERT_EQ(std::vector<std::string>{" "}, h.commands());
  ASSERT_EQ(data::empty_container(), d.raw_texts());
  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_TRUE(d.call_graphs().empty());

  sh.line_available("\t", d, h);
  ASSERT_EQ((std::vector<std::string>{" ", "\t"}), h.commands());
  ASSERT_EQ(data::empty_container(), d.raw_texts());
  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_TRUE(d.call_graphs().empty());
}

TEST(mdb_shell, identical_lines_in_history)
{
  core::null_displayer d;
  core::in_memory_history h;
  test_shell sh;

  ASSERT_EQ(data::empty_container(), h.commands());

  sh.line_available("asd", d, h);
  ASSERT_EQ(std::vector<std::string>{"asd"}, h.commands());

  sh.line_available("asd", d, h);
  ASSERT_EQ(std::vector<std::string>{"asd"}, h.commands());

  sh.line_available("xyz", d, h);
  ASSERT_EQ((std::vector<std::string>{"asd", "xyz"}), h.commands());

  sh.line_available("asd", d, h);
  ASSERT_EQ((std::vector<std::string>{"asd", "xyz", "asd"}), h.commands());
}

TEST(mdb_shell, identical_all_space_lines_in_history)
{
  core::null_displayer d;
  core::in_memory_history h;
  test_shell sh;

  ASSERT_EQ(data::empty_container(), h.commands());

  sh.line_available(" ", d, h);
  ASSERT_EQ(std::vector<std::string>{" "}, h.commands());

  sh.line_available(" ", d, h);
  ASSERT_EQ(std::vector<std::string>{" "}, h.commands());

  sh.line_available("  ", d, h);
  ASSERT_EQ((std::vector<std::string>{" ", "  "}), h.commands());

  sh.line_available(" ", d, h);
  ASSERT_EQ((std::vector<std::string>{" ", "  ", " "}), h.commands());
}

TEST(mdb_shell, skips_empty_lines)
{
  core::null_displayer d;
  core::in_memory_history h;
  test_shell sh;

  ASSERT_EQ(data::empty_container(), h.commands());

  sh.line_available("ads", d, h);
  ASSERT_EQ(std::vector<std::string>{"ads"}, h.commands());

  sh.line_available("", d, h);
  ASSERT_EQ(std::vector<std::string>{"ads"}, h.commands());

  sh.line_available("ads", d, h);
  ASSERT_EQ(std::vector<std::string>{"ads"}, h.commands());
}

TEST(mdb_shell, prompt)
{
  test_shell sh;

  ASSERT_EQ(sh.prompt(), "(mdb)");
}

TEST(mdb_shell, display_splash)
{
  test_shell sh;

  core::in_memory_displayer d;
  sh.display_splash(d);

  ASSERT_EQ(
      std::vector<std::string>{"For help, type \"help\"."}, d.raw_texts());
}
