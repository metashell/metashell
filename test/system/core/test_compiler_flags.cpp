// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/type.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  constexpr char define_d[] =
      " template <int N> struct d : d<N - 1> {};"
      " template <> struct d<0> { typedef int type; };";
}

TEST(compiler_flags, template_instantiation_depth)
{
  if (!using_wave() && !using_msvc())
  {
    metashell_instance mi1024({"--", "-ftemplate-depth=1024"});
    mi1024.command(define_d);

    metashell_instance mi32({"--", "-ftemplate-depth=32"});
    mi32.command(define_d);

    ASSERT_EQ(error(_), mi32.command("d<512>::type").front());
    ASSERT_EQ(type("int"), mi1024.command("d<512>::type").front());
  }
}
