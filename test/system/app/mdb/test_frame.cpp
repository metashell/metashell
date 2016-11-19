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
#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>
#include <metashell/system_test/type.hpp>

#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(mdb_frame, without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      error("Metaprogram not evaluated yet"), mi.command("frame").front());
}

TEST(mdb_frame, before_starting)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  ASSERT_EQ(frame(type("int")), mi.command("frame 0").front());
}

TEST(mdb_frame, fib_step_1)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("step");

  ASSERT_EQ(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("frame 0").front());

  ASSERT_EQ(frame(type("int_<fib<10>::value>")), mi.command("frame 1").front());
}

TEST(mdb_frame, fib_step_2)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("step 2");

  ASSERT_EQ(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      mi.command("frame 0").front());

  ASSERT_EQ(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("frame 1").front());

  ASSERT_EQ(frame(type("int_<fib<10>::value>")), mi.command("frame 2").front());
}

TEST(mdb_frame, fib_at_end)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  ASSERT_EQ(raw_text("Metaprogram finished"), mi.command("continue").front());
  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(type("int")), to_json_string(prompt("(mdb)"))}),
            mi.command("frame 0"));
}

TEST(mdb_frame, fib_no_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  ASSERT_EQ(error("Argument parsing failed"), mi.command("frame").front());
}

TEST(mdb_frame, fib_garbage_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  ASSERT_EQ(error("Argument parsing failed"), mi.command("frame asd").front());
}

TEST(mdb_frame, fib_out_of_range_arg_negative)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  ASSERT_EQ(error("Frame index out of range"), mi.command("frame -1").front());
}

TEST(mdb_frame, fib_out_of_range_arg_too_large)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  ASSERT_EQ(error("Frame index out of range"), mi.command("frame 1").front());
}
