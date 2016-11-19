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

TEST(mdb_next, without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(error("Metaprogram not evaluated yet"), mi.command("next").front());
}

TEST(mdb_next, garbage_argument)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<2>::value>");

  ASSERT_EQ(error("Argument parsing failed"), mi.command("next asd").front());
}

TEST(mdb_next, fib_from_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  ASSERT_EQ(
      (std::vector<json_string>{
          to_json_string(raw_text("Metaprogram finished")),
          to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))}),
      mi.command("next"));
}

TEST(mdb_next, minus_1_multi_fib_from_after_step)
{
  metashell_instance mi;
  mi.command(multi_fibonacci_mp);
  mi.command("#msh mdb int_<multi_fib<10>::value>");

  ASSERT_EQ(frame(type("multi_fib<4>"), _, _, instantiation_kind::memoization),
            mi.command("step 15").front());

  ASSERT_EQ(frame(type("multi_fib<5>"), _, _, instantiation_kind::memoization),
            mi.command("next").front());

  ASSERT_EQ(frame(type("multi_fib<6>"), _, _, instantiation_kind::memoization),
            mi.command("next").front());

  ASSERT_EQ(frame(type("multi_fib<6>"), _, _,
                  instantiation_kind::template_instantiation),
            mi.command("next -1").front());
}
