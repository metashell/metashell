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

using namespace metashell_system_test;

namespace
{
  template <class It, class T>
  It find_last(It begin_, It end_, T val_)
  {
    if (begin_ == end_)
    {
      return end_;
    }
    else
    {
      for (It i = end_ - 1; i != begin_; --i)
      {
        if (*i == val_)
        {
          return i;
        }
      }
      return *begin_ == val_ ? begin_ : end_;
    }
  }
}

int metashell_system_test::main(int argc_, char* argv_[])
{
  if (argc_ < 2)
  {
    std::cerr
        << "Usage: " << argv_[0]
        << " <metashell_binary> <metashell arguments> -- <just::test arguments>"
        << std::endl;
    return -1;
  }
  else
  {
    const auto end = argv_ + argc_;
    const auto sep = find_last(argv_, end, std::string("--"));

    std::cerr << "Testing the following Metashell command:";
    for (auto i = argv_ + 1; i != sep; ++i)
    {
      std::cerr << " " << *i;
    }
    std::cerr << std::endl;

    system_test_config::metashell_binary(argv_[1]);
    for (auto i = argv_ + 2; i != sep; ++i)
    {
      system_test_config::metashell_arg(*i);
    }

    std::vector<char*> just_test_args = {argv_[0]};
    if (sep != end)
    {
      just_test_args.insert(just_test_args.end(), sep + 1, end);
    }

    return ::just::test::run(just_test_args.size(), just_test_args.data());
  }
}
