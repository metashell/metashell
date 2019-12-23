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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;
using namespace metashell;
using pattern::_;

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

    ASSERT_EQ(comment("Already using engine " + engine_),
              mi_.command("#msh engine switch " + engine_).front());

    if (engine_ == "null")
    {
      for (const std::string engine :
           {"clang", "gcc", "internal", "msvc", "null", "pure_wave",
            "templight", "wave"})
      {
        if (engine != engine_)
        {
          ASSERT_EQ(error("Error switching to engine " + engine +
                          ": Switching from engine " + engine_ +
                          " is not supported."),
                    mi_.command("#msh engine switch " + engine).front());
        }
      }
    }
    else
    {
      for (const std::string engine : {"clang", "gcc", "msvc", "templight"})
      {
        if (engine != engine_)
        {
          ASSERT_EQ(
              error("Error switching to engine " + engine +
                    ": Switching to engine " + engine + " is not supported."),
              mi_.command("#msh engine switch " + engine).front());
        }
      }

      for (const std::string engine : {"internal", "null", "pure_wave", "wave"})
      {
        if (engine != engine_)
        {
          const json_string result =
              mi_.command("#msh engine switch " + engine).front();
          ASSERT_EQ(comment(_), result);
        }
      }
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

TEST(engine_switch, unsupported_compiler_arguments)
{
  const std::string engine = current_real_engine();
  if (engine != "wave" && engine != "pure_wave")
  {
    metashell_instance mi({"--no_precompiled_headers", "--", "/FOOBAR"});
    ASSERT_EQ(error("Error switching to engine wave: Compiler argument /FOOBAR "
                    "is not yet supported by Metashell."),
              mi.command("#msh engine switch wave").front());
  }
}
