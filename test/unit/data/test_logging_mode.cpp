// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/logging_mode.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace metashell::data;

namespace
{
  std::string display(logging_mode mode_)
  {
    std::ostringstream s;
    s << mode_;
    return s.str();
  }
} // namespace

TEST(logging_mode, displayer)
{
  ASSERT_EQ("none", display(logging_mode::none));
  ASSERT_EQ("console", display(logging_mode::console));
  ASSERT_EQ("file", display(logging_mode::file));
}
