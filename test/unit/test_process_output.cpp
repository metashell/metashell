// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/process_output.hpp>

#include <gtest/gtest.h>

using namespace metashell::data;

TEST(process_output, dos2unix)
{
  const std::string dos = "hello\r\ndos\rworld\n";
  const std::string unix = "hello\ndos\rworld\n";

  const process_output result1 = dos2unix({exit_code_t(1), dos, ""});
  const process_output result2 = dos2unix({exit_code_t(1), "", dos});

  ASSERT_EQ(unix, result1.standard_output);
  ASSERT_EQ("", result1.standard_error);

  ASSERT_EQ("", result2.standard_output);
  ASSERT_EQ(unix, result2.standard_error);
}
