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

#include "replace_part.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

int main(int argc_, char* argv_[])
{
  if (argc_ < 2)
  {
    std::cerr << "Usage: " << argv_[0] << " <path to replace_part>"
              << std::endl;
    return 1;
  }

  replace_part::path(argv_[1]);

  std::vector<char*> gtest_args = {argv_[0]};
  gtest_args.insert(gtest_args.end(), argv_ + 2, argv_ + argc_);
  int argc = gtest_args.size();
  ::testing::InitGoogleTest(&argc, gtest_args.data());
  return RUN_ALL_TESTS();
}
