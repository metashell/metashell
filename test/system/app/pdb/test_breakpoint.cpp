// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/nocaches.hpp>
#include <metashell/system_test/raw_text.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(breakpoint, rbreak_stop_times)
{
  for (const std::string& nocache : nocaches())
  {
    metashell_instance mi;
    mi.command("#define FOO(x) x");
    mi.command("#msh pdb" + nocache + " FOO(31)");

    ASSERT_EQ(
        raw_text(
            caching_enabled(nocache) ?
                "Breakpoint \"31\" will stop the execution on 4 locations" :
                "Breakpoint \"31\" created"),
        mi.command("rbreak 31").front());
  }
}
