// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/code_completer.hpp>
#include <metashell/system_test/code_completion_result.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

namespace
{
  auto res = code_completion_result::create;
}

TEST(pragma_pdb, code_complete)
{
  // further test cases are covered by test_completion.cpp

  code_completer c;

  ASSERT_EQ(c("#msh pdb"), res(" -", " -profile", " -nocache"));
  ASSERT_EQ(c("#msh pdb "), res("-", "-profile", "-nocache"));
  ASSERT_EQ(c("#msh pdb -"), res("", "profile", "nocache"));
  ASSERT_EQ(c("#msh pdb -n"), res("ocache"));
  ASSERT_EQ(c("#msh pdb -no"), res("cache"));
  ASSERT_EQ(c("#msh pdb -noc"), res("ache"));
  ASSERT_EQ(c("#msh pdb -noca"), res("che"));
  ASSERT_EQ(c("#msh pdb -nocac"), res("he"));
  ASSERT_EQ(c("#msh pdb -nocach"), res("e"));
  ASSERT_EQ(c("#msh pdb -nocache"), res(""));
  ASSERT_EQ(c("#msh pdb -nocache -"), res("", "profile"));
  ASSERT_EQ(c("#msh pdb -nocache -p"), res("rofile"));
  ASSERT_EQ(c("#msh pdb -nocache -pr"), res("ofile"));
  ASSERT_EQ(c("#msh pdb -nocache -pro"), res("file"));
  ASSERT_EQ(c("#msh pdb -nocache -prof"), res("ile"));
  ASSERT_EQ(c("#msh pdb -nocache -profi"), res("le"));
  ASSERT_EQ(c("#msh pdb -nocache -profil"), res("e"));
  ASSERT_EQ(c("#msh pdb -nocache -profile"), res(""));
  ASSERT_EQ(c("#msh pdb -nocache -profile -"), res(""));
  ASSERT_EQ(c("#msh pdb -p"), res("rofile"));
  ASSERT_EQ(c("#msh pdb -pr"), res("ofile"));
  ASSERT_EQ(c("#msh pdb -pro"), res("file"));
  ASSERT_EQ(c("#msh pdb -prof"), res("ile"));
  ASSERT_EQ(c("#msh pdb -profi"), res("le"));
  ASSERT_EQ(c("#msh pdb -profil"), res("e"));
  ASSERT_EQ(c("#msh pdb -profile"), res(""));
  ASSERT_EQ(c("#msh pdb -profile -"), res("", "nocache"));
  ASSERT_EQ(c("#msh pdb -profile -n"), res("ocache"));
  ASSERT_EQ(c("#msh pdb -profile -no"), res("cache"));
  ASSERT_EQ(c("#msh pdb -profile -noc"), res("ache"));
  ASSERT_EQ(c("#msh pdb -profile -noca"), res("che"));
  ASSERT_EQ(c("#msh pdb -profile -nocac"), res("he"));
  ASSERT_EQ(c("#msh pdb -profile -nocach"), res("e"));
  ASSERT_EQ(c("#msh pdb -profile -nocache"), res(""));
  ASSERT_EQ(c("#msh pdb -profile -nocache -"), res(""));

  ASSERT_EQ(c("#msh pdb - -prof"), res());
  ASSERT_EQ(c("#msh pdb - doubl"), res());
}
