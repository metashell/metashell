// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include "util.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

std::tuple<metashell::mdb_command, std::string>
get_command_from_map(const metashell::mdb_command_handler_map& map,
                     const std::string& line)
{
  if (const auto opt_pair = map.get_command_for_line(line))
  {
    return *opt_pair;
  }
  else
  {
    throw std::logic_error("Command for " + line + " not found.");
  }
}
