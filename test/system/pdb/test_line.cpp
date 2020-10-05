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

#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/util.hpp>

#include <gtest/gtest.h>

#include <just/temp.hpp>

#include <boost/filesystem/path.hpp>

#include <fstream>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  json_string ft_of(const std::string& exp_)
  {
    metashell_instance mi;
    mi.command("#msh pdb " + exp_);
    return mi.command("ft").front();
  }
} // namespace

TEST(pdb, line_in_trace)
{
  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("#line 21")), 0, 2},
        {frame(type("#line 21"), _, _, event_kind::line_directive), 1, 0},
        {frame(type("\\n"), _, _, event_kind::skipped_token), 1, 0}
    }),
    ft_of("#line 21")
  );

  ASSERT_EQ(
    call_graph({
      {frame(type("#line 31 \"bar.hpp\"")), 0, 2},
        {frame(type("#line 31 \"bar.hpp\""), _, _, event_kind::line_directive), 1, 0},
        {frame(type("\\n"), _, _, event_kind::skipped_token), 1, 0}
    }),
    ft_of("#line 31 \"bar.hpp\"")
  );

  // clang-format on
}
