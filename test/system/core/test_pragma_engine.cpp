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
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(pragma_engine, show)
{
  metashell_instance mi;

  const auto result = mi.command("#msh engine show");

  ASSERT_EQ(2u, result.size());

  ASSERT_EQ(comment(_), result.front());
  ASSERT_TRUE(result.front().get().find("\\\"name\\\":") != std::string::npos);
  ASSERT_TRUE(result.front().get().find("\\\"display_name\\\":") !=
              std::string::npos);

  ASSERT_EQ(prompt(">"), result.back());
}
