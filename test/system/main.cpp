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

#include "system_test_config.hpp"

#include <just/test.hpp>

#include <iostream>
#include <vector>

int main(int argc_, char* argv_[])
{
  if (argc_ < 3 || argv_[2] != std::string("--"))
  {
    std::cerr
      << "Usage: " << argv_[0]
      << " <metashell_binary> -- <just::test arguments>" << std::endl;
    return -1;
  }
  else
  {
    system_test_config::metashell_binary(argv_[1]);

    std::vector<char*> just_test_args(argv_ + 2, argv_ + argc_);
    just_test_args[0] = argv_[0];

    return ::just::test::run(just_test_args.size(), just_test_args.data());
  }
}

