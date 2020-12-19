#ifndef METASHELL_SYSTEM_TEST_MDB_FIB_H
#define METASHELL_SYSTEM_TEST_MDB_FIB_H

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

#include <metashell/system_test/type.hpp>

#include <string>

namespace impl
{
  template <int Ver, int N>
  struct fib
  {
    static metashell::system_test::type name()
    {
      const std::string ns = Ver > 0 ? "v" + std::to_string(Ver) + "::" : "";
      return metashell::system_test::type{ns + "fib<" + std::to_string(N) +
                                          ">"};
    }

    operator metashell::system_test::type() const { return name(); }

    struct value
    {
      operator metashell::system_test::type() const
      {
        return metashell::system_test::type{fib<Ver, N>::name().name() +
                                            "::value"};
      }
    };
  };
} // namespace impl

template <int N>
using fib = ::impl::fib<-1, N>;

namespace v1
{
  template <int N>
  using fib = ::impl::fib<1, N>;
} // namespace v1

namespace v2
{
  template <int N>
  using fib = ::impl::fib<2, N>;
} // namespace v2

#endif
