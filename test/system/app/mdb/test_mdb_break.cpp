// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>

#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(mdb_break, list_with_no_breakpoints)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(raw_text("No breakpoints currently set"),
            mi.command("break list").front());
}

TEST(mdb_break, list_with_one_breakpoint)
{
  metashell_instance mi;
  mi.command("#msh mdb int");
  mi.command("rbreak i");

  ASSERT_EQ(
      raw_text("Breakpoint 1: regex(\"i\")"), mi.command("break list").front());
}

TEST(mdb_break, list_with_two_breakpoints)
{
  metashell_instance mi;
  mi.command("#msh mdb int");
  mi.command("rbreak i");
  mi.command("rbreak n");

  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Breakpoint 1: regex(\"i\")")),
                to_json_string(raw_text("Breakpoint 2: regex(\"n\")")),
                to_json_string(prompt("(mdb)"))}),
            mi.command("break list"));
}

TEST(mdb_break, garbage_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(error("Call break like this: \"break list\""),
            mi.command("break asd").front());
}
