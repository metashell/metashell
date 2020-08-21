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

TEST(pragma_mdb, code_complete)
{
  // further test cases are covered by test_completion.cpp

  code_completer c;

  ASSERT_EQ(c("#msh mdb"), res(" -", " -full", " -profile", " -nocache"));
  ASSERT_EQ(c("#msh mdb "), res("-", "-full", "-profile", "-nocache"));
  ASSERT_EQ(c("#msh mdb -"), res("", "full", "profile", "nocache"));
  ASSERT_EQ(c("#msh mdb -f"), res("ull"));
  ASSERT_EQ(c("#msh mdb -fu"), res("ll"));
  ASSERT_EQ(c("#msh mdb -ful"), res("l"));
  ASSERT_EQ(c("#msh mdb -full"), res(""));
  ASSERT_EQ(c("#msh mdb -full -"), res("", "nocache"));
  ASSERT_EQ(c("#msh mdb -full -n"), res("ocache"));
  ASSERT_EQ(c("#msh mdb -full -no"), res("cache"));
  ASSERT_EQ(c("#msh mdb -full -noc"), res("ache"));
  ASSERT_EQ(c("#msh mdb -full -noca"), res("che"));
  ASSERT_EQ(c("#msh mdb -full -nocac"), res("he"));
  ASSERT_EQ(c("#msh mdb -full -nocach"), res("e"));
  ASSERT_EQ(c("#msh mdb -full -nocache"), res(""));
  ASSERT_EQ(c("#msh mdb -full -nocache -"), res(""));
  ASSERT_EQ(c("#msh mdb -n"), res("ocache"));
  ASSERT_EQ(c("#msh mdb -no"), res("cache"));
  ASSERT_EQ(c("#msh mdb -noc"), res("ache"));
  ASSERT_EQ(c("#msh mdb -noca"), res("che"));
  ASSERT_EQ(c("#msh mdb -nocac"), res("he"));
  ASSERT_EQ(c("#msh mdb -nocach"), res("e"));
  ASSERT_EQ(c("#msh mdb -nocache"), res(""));
  ASSERT_EQ(c("#msh mdb -nocache -"), res("", "full", "profile"));
  ASSERT_EQ(c("#msh mdb -nocache -f"), res("ull"));
  ASSERT_EQ(c("#msh mdb -nocache -fu"), res("ll"));
  ASSERT_EQ(c("#msh mdb -nocache -ful"), res("l"));
  ASSERT_EQ(c("#msh mdb -nocache -full"), res(""));
  ASSERT_EQ(c("#msh mdb -nocache -full -"), res(""));
  ASSERT_EQ(c("#msh mdb -nocache -p"), res("rofile"));
  ASSERT_EQ(c("#msh mdb -nocache -pr"), res("ofile"));
  ASSERT_EQ(c("#msh mdb -nocache -pro"), res("file"));
  ASSERT_EQ(c("#msh mdb -nocache -prof"), res("ile"));
  ASSERT_EQ(c("#msh mdb -nocache -profi"), res("le"));
  ASSERT_EQ(c("#msh mdb -nocache -profil"), res("e"));
  ASSERT_EQ(c("#msh mdb -nocache -profile"), res(""));
  ASSERT_EQ(c("#msh mdb -nocache -profile -"), res(""));
  ASSERT_EQ(c("#msh mdb -p"), res("rofile"));
  ASSERT_EQ(c("#msh mdb -pr"), res("ofile"));
  ASSERT_EQ(c("#msh mdb -pro"), res("file"));
  ASSERT_EQ(c("#msh mdb -prof"), res("ile"));
  ASSERT_EQ(c("#msh mdb -profi"), res("le"));
  ASSERT_EQ(c("#msh mdb -profil"), res("e"));
  ASSERT_EQ(c("#msh mdb -profile"), res(""));
  ASSERT_EQ(c("#msh mdb -profile -"), res("", "nocache"));
  ASSERT_EQ(c("#msh mdb -profile -n"), res("ocache"));
  ASSERT_EQ(c("#msh mdb -profile -no"), res("cache"));
  ASSERT_EQ(c("#msh mdb -profile -noc"), res("ache"));
  ASSERT_EQ(c("#msh mdb -profile -noca"), res("che"));
  ASSERT_EQ(c("#msh mdb -profile -nocac"), res("he"));
  ASSERT_EQ(c("#msh mdb -profile -nocach"), res("e"));
  ASSERT_EQ(c("#msh mdb -profile -nocache"), res(""));
  ASSERT_EQ(c("#msh mdb -profile -nocache -"), res(""));

  ASSERT_EQ(c("#msh mdb - -prof"), res());
  ASSERT_EQ(c("#msh mdb - doubl"), res());
}
