// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <gtest/gtest.h>

using namespace metashell::system_test;
using namespace metashell;

namespace
{
  void test_error_handling(const std::string& engine_, metashell_instance& mi_)
  {
    ASSERT_EQ(error("Usage: engine switch <engine name>"),
              mi_.command("#msh engine switch").front());

    ASSERT_EQ(error("Error: Invalid engine name: asd"),
              mi_.command("#msh engine switch asd").front());

    ASSERT_EQ(error("Error: Invalid engine name: auto"),
              mi_.command("#msh engine switch auto").front());

    for (const std::string engine : {"clang", "gcc", "internal", "msvc", "null",
                                     "pure_wave", "templight", "wave"})
    {
      ASSERT_EQ(
          error("Error switching to engine " + engine +
                ": Switching from engine " + engine_ + " is not supported."),
          mi_.command("#msh engine switch " + engine).front());
    }
  }
}

TEST(engine_switch, current_engine)
{
  metashell_instance mi;
  test_error_handling(current_real_engine(), mi);
}

TEST(engine_switch, null_engine)
{
  metashell_instance mi({"--engine", "null"}, {}, false);
  test_error_handling("null", mi);
}
