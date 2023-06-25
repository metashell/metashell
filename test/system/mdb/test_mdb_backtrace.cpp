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

#include <metashell/system_test/any_of.hpp>
#include <metashell/system_test/backtrace.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/nocaches.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>

#include "fib.hpp"
#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(mdb_backtrace, without_evaluation)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb" + nocache);

    ASSERT_EQ(error("Metaprogram not evaluated yet"),
              mi.command("backtrace").front());
  }
}

TEST(mdb_backtrace, unstepped_fibonacci)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<10>::value>");

    ASSERT_EQ(backtrace{{frame{type{"int_<fib<10>::value>"}}}},
              mi.command("backtrace").front());
  }
}

TEST(mdb_backtrace, metaprogram_finished)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int");

    ASSERT_EQ(raw_text("Metaprogram finished"), mi.command("continue").front());
    ASSERT_EQ(
        (std::vector<json_string>{
            to_json_string(raw_text("Metaprogram finished")),
            to_json_string(type{"int"}), to_json_string(prompt("(mdb)"))}),
        mi.command("backtrace"));
  }
}

TEST(mdb_backtrace, metaprogram_finished_in_full_mode)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " -full int");

    ASSERT_EQ(raw_text("Metaprogram finished"), mi.command("continue").front());
    ASSERT_EQ(
        (std::vector<json_string>{
            to_json_string(raw_text("Metaprogram finished")),
            to_json_string(type{"int"}), to_json_string(prompt("(mdb)"))}),
        mi.command("backtrace"));
  }
}

TEST(mdb_backtrace, one_stepped_fibonacci)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<10>::value>");
    mi.command("step");

    ASSERT_EQ(
        backtrace({frame{fib<10>{}, _, _, event_kind::template_instantiation},
                   frame{type{"int_<fib<10>::value>"}}}),
        mi.command("backtrace").front());
  }
}

TEST(mdb_backtrace, two_stepped_fibonacci)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<10>::value>");
    mi.command("step 2");

    ASSERT_EQ(
        any_of<backtrace>(
            backtrace{
                {frame{fib<8>{}, _, _, event_kind::template_instantiation},
                 frame{fib<10>{}, _, _, event_kind::template_instantiation},
                 frame{type{"int_<fib<10>::value>"}}}},
            backtrace{{frame{fib<10>{}, _, _, event_kind::memoization},
                       frame{type{"int_<fib<10>::value>"}}}}),
        mi.command("backtrace").front());
  }
}

TEST(mdb_backtrace, three_stepped_fibonacci)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<10>::value>");
    mi.command("step 3");

    ASSERT_EQ(
        any_of<backtrace>(
            backtrace{
                {frame{fib<6>{}, _, _, event_kind::template_instantiation},
                 frame{fib<8>{}, _, _, event_kind::template_instantiation},
                 frame{fib<10>{}, _, _, event_kind::template_instantiation},
                 frame{type{"int_<fib<10>::value>"}}}},
            backtrace{{frame{type{"fib<10>::value"}, _, _,
                             event_kind::template_instantiation},
                       frame{type{"int_<fib<10>::value>"}}}}),
        mi.command("backtrace").front());
  }
}

TEST(mdb_backtrace, garbage_argument)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<10>::value>");

    ASSERT_EQ(error("This command doesn't accept arguments"),
              mi.command("backtrace asd").front());
  }
}

TEST(mdb_backtrace, bt_alias)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<10>::value>");
    mi.command("step");

    ASSERT_EQ(
        backtrace({frame{fib<10>{}, _, _, event_kind::template_instantiation},
                   frame{type{"int_<fib<10>::value>"}}}),
        mi.command("backtrace").front());

    ASSERT_EQ(
        backtrace({frame{fib<10>{}, _, _, event_kind::template_instantiation},
                   frame{type{"int_<fib<10>::value>"}}}),
        mi.command("bt").front());
  }
}

TEST(mdb_backtrace, on_error)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command(missing_value_fibonacci_mp);
    mi.command("#msh mdb");
    mi.command("evaluate" + nocache + " int_<fib<5>::value>");
    mi.command("continue");

    ASSERT_EQ(
        any_of<backtrace>(
            backtrace{{frame{fib<3>{}, _, _, event_kind::memoization},
                       frame{fib<4>::value{}, _, _,
                             event_kind::template_instantiation},
                       frame{fib<5>::value{}, _, _,
                             event_kind::template_instantiation},
                       frame{type{"int_<fib<5>::value>"}}}},
            backtrace{
                {frame{fib<0>{}, _, _, event_kind::memoization},
                 frame{fib<2>{}, _, _, event_kind::template_instantiation},
                 frame{fib<3>{}, _, _, event_kind::template_instantiation},
                 frame{fib<5>{}, _, _, event_kind::template_instantiation},
                 frame{type{"int_<fib<5>::value>"}}}},
            backtrace{{frame{fib<0>{}, _, _, event_kind::memoization},
                       frame{fib<2>::value{}, _, _,
                             event_kind::template_instantiation},
                       frame{fib<3>::value{}, _, _,
                             event_kind::template_instantiation},
                       frame{fib<5>::value{}, _, _,
                             event_kind::template_instantiation},
                       frame{type{"int_<fib<5>::value>"}}}}),
        mi.command("backtrace").front());
  }
}
