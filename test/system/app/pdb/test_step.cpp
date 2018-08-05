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

#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/nocaches.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(step, step_over_in_macro_evaluation)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#define FOO(x) x");
    mi.command("#msh pdb" + nocache + " FOO(31)");

    ASSERT_EQ(frame(type("FOO(31)")), mi.command("step").front());
    ASSERT_EQ(frame(type("31")), mi.command("step over").front());
  }
}
