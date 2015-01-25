// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/main.hpp>
#include <metashell_system_test/system_test_config.hpp>

#include <just/test.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace metashell_system_test;

int metashell_system_test::main(int argc_, char* argv_[])
{
  const auto end = argv_ + argc_;
  const auto sep = std::find(argv_, end, std::string("--"));

  if (argc_ < 3 || sep == end)
  {
    std::cerr
      << "Usage: " << argv_[0]
      << " <metashell_binary> <metashell arguments> -- <just::test arguments>"
      << std::endl;
    return -1;
  }
  else
  {
    system_test_config::metashell_binary(argv_[1]);
    for (auto i = argv_ + 2; i != sep; ++i)
    {
      system_test_config::metashell_arg(*i);
    }

    // starting from sep, as it will the "name of the binary" argument
    std::vector<char*> just_test_args(sep, end);
    just_test_args[0] = argv_[0];

    return ::just::test::run(just_test_args.size(), just_test_args.data());
  }
}

