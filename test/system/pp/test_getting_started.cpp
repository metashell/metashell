// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <pattern/regex.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::regex;

// If one of these TCs fail, then getting_started.md modification might be
// needed

namespace
{
  error macro_discovery_not_supported(const std::string& engine_)
  {
    return error("Error: Feature macro discovery is not supported by the " +
                 engine_ + " engine.");
  }
} // namespace

TEST(getting_started, testing_macros)
{
  metashell_instance mi;

  mi.command("#msh preprocessor mode");
  mi.command("#include <boost/preprocessor/arithmetic/add.hpp>");

  ASSERT_EQ(cpp_code("13"), mi.command("BOOST_PP_ADD(6, 7)").front());

  mi.command("#include <boost/preprocessor/config/limits.hpp>");

  ASSERT_EQ(
      cpp_code(regex("[0-9]+")), mi.command("BOOST_PP_LIMIT_MAG").front());

  const auto macro_names = mi.command("#msh macro names").front();

  if (macro_names != macro_discovery_not_supported("msvc") &&
      macro_names != macro_discovery_not_supported("auto"))
  {
    ASSERT_EQ(cpp_code(regex(".*\nBOOST_PP_LIMIT_MAG\n.*")), macro_names);

    ASSERT_EQ(cpp_code(regex(".*\n#define BOOST_PP_LIMIT_MAG [0-9]+\n.*")),
              mi.command("#msh macros").front());
  }
}
