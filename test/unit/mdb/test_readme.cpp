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

#include <metashell/mdb/shell.hpp>

#include <gtest/gtest.h>

#include "util.hpp"

using namespace metashell;

// If one of these TCs fail, then README modification might be needed

TEST(readme, continue_abbreviated_as_c)
{
  const std::optional<data::mdb_command> c = data::mdb_command::parse("c");
  ASSERT_TRUE(c);

  const mdb::command command =
      get_command_from_map(mdb::shell::build_command_handler(false), *c);
  const auto keys = command.get_keys();

  ASSERT_TRUE(std::find(keys.begin(), keys.end(),
                        data::mdb_command::name_type("continue")) !=
              keys.end());
}
