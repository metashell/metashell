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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/raw_text.hpp>

#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(mdb_rbreak, without_evaluated_metaprogram)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      error("Metaprogram not evaluated yet"), mi.command("rbreak int").front());
}

TEST(mdb_rbreak, no_arguments)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  ASSERT_EQ(error("Argument expected"), mi.command("rbreak").front());
}

TEST(mdb_rbreak, no_arguments_with_trailing_whitespace)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  ASSERT_EQ(error("Argument expected"), mi.command("rbreak ").front());
}

TEST(mdb_rbreak, invalid_regex)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  ASSERT_EQ(
      error("\"[\" is not a valid regex"), mi.command("rbreak [").front());
}

TEST(mdb_rbreak, valid_regex_no_match)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  ASSERT_EQ(raw_text("Breakpoint \"xyz\" will never stop the execution"),
            mi.command("rbreak xyz").front());
}

TEST(mdb_rbreak, valid_regex_with_one_match)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  ASSERT_EQ(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"),
      mi.command("rbreak int").front());
}

TEST(mdb_rbreak, valid_regex_with_three_matches)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");

  ASSERT_EQ(
      raw_text("Breakpoint \"fib<3>\" will stop the execution on 3 locations"),
      mi.command("rbreak fib<3>").front());
}

TEST(mdb_rbreak, does_not_count_stops_in_unreachable_subgraphs)
{
  metashell_instance mi;
  mi.command(fibonacci_mp + "int __x = fib<10>::value;");
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<2>::value>");

  ASSERT_EQ(
      raw_text("Breakpoint \"fib\" will stop the execution on 3 locations"),
      mi.command("rbreak fib").front());
}

TEST(mdb_rbreak, valid_regex_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate -full int_<fib<5>::value>");

  ASSERT_EQ(
      raw_text("Breakpoint \"fib<3>\" will stop the execution on 2 locations"),
      mi.command("rbreak fib<3>").front());
}

TEST(mdb_rbreak, valid_regex_in_full_mode_match_only_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate -full int_<fib<5>::value>");

  ASSERT_EQ(
      raw_text(
          "Breakpoint \"int_<fib<5>::value>\" will never stop the execution"),
      mi.command("rbreak int_<fib<5>::value>").front());
}

TEST(mdb_rbreak, valid_regex_in_full_mode_match_also_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate -full int_<fib<5>::value>");

  ASSERT_EQ(raw_text("Breakpoint \"(int_<fib<5>::value>)|(fib<3>)\" "
                     "will stop the execution on 2 locations"),
            mi.command("rbreak (int_<fib<5>::value>)|(fib<3>)").front());
}
