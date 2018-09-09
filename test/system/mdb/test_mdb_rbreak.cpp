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
#include <metashell/system_test/nocaches.hpp>
#include <metashell/system_test/raw_text.hpp>

#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(mdb_rbreak, without_evaluated_metaprogram)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb" + nocache);

    ASSERT_EQ(error("Metaprogram not evaluated yet"),
              mi.command("rbreak int").front());
  }
}

TEST(mdb_rbreak, no_arguments)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int");

    ASSERT_EQ(error("Argument expected"), mi.command("rbreak").front());
  }
}

TEST(mdb_rbreak, no_arguments_with_trailing_whitespace)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int");

    ASSERT_EQ(error("Argument expected"), mi.command("rbreak ").front());
  }
}

TEST(mdb_rbreak, invalid_regex)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int");

    ASSERT_EQ(
        error("\"[\" is not a valid regex"), mi.command("rbreak [").front());
  }
}

TEST(mdb_rbreak, valid_regex_no_match)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int");

    ASSERT_EQ(raw_text(caching_enabled(nocache) ?
                           "Breakpoint \"xyz\" will never stop the execution" :
                           "Breakpoint \"xyz\" created"),
              mi.command("rbreak xyz").front());
  }
}

TEST(mdb_rbreak, valid_regex_with_one_match)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int");

    ASSERT_EQ(
        raw_text(
            caching_enabled(nocache) ?
                "Breakpoint \"int\" will stop the execution on 1 location" :
                "Breakpoint \"int\" created"),
        mi.command("rbreak int").front());
  }
}

TEST(mdb_rbreak, valid_regex_with_three_matches)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<5>::value>");

    ASSERT_EQ(
        raw_text(
            caching_enabled(nocache) ?
                "Breakpoint \"fib<3>\" will stop the execution on 3 locations" :
                "Breakpoint \"fib<3>\" created"),
        mi.command("rbreak fib<3>").front());
  }
}

TEST(mdb_rbreak, does_not_count_stops_in_unreachable_subgraphs)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp + "int __x = fib<10>::value;");
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<2>::value>");

    ASSERT_EQ(
        raw_text(
            caching_enabled(nocache) ?
                "Breakpoint \"fib\" will stop the execution on 3 locations" :
                "Breakpoint \"fib\" created"),
        mi.command("rbreak fib").front());
  }
}

TEST(mdb_rbreak, valid_regex_in_full_mode)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " -full int_<fib<5>::value>");

    ASSERT_EQ(
        raw_text(
            caching_enabled(nocache) ?
                "Breakpoint \"fib<3>\" will stop the execution on 2 locations" :
                "Breakpoint \"fib<3>\" created"),
        mi.command("rbreak fib<3>").front());
  }
}

TEST(mdb_rbreak, valid_regex_in_full_mode_match_only_root)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " -full int_<fib<5>::value>");

    ASSERT_EQ(raw_text(caching_enabled(nocache) ?
                           "Breakpoint \"int_<fib<5>::value>\" will never stop "
                           "the execution" :
                           "Breakpoint \"int_<fib<5>::value>\" created"),
              mi.command("rbreak int_<fib<5>::value>").front());
  }
}

TEST(mdb_rbreak, valid_regex_in_full_mode_match_also_root)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " -full int_<fib<5>::value>");

    ASSERT_EQ(
        raw_text(caching_enabled(nocache) ?
                     "Breakpoint \"(int_<fib<5>::value>)|(fib<3>)\" "
                     "will stop the execution on 2 locations" :
                     "Breakpoint \"(int_<fib<5>::value>)|(fib<3>)\" created"),
        mi.command("rbreak (int_<fib<5>::value>)|(fib<3>)").front());
  }
}
