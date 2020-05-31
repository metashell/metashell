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

#include <metashell/system_test/code_completer.hpp>
#include <metashell/system_test/code_completion_result.hpp>
#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  auto res = code_completion_result::create;
}

TEST(pragma_engine, show)
{
  metashell_instance mi;

  const auto result = mi.command("#msh engine show");

  ASSERT_EQ(2u, result.size());

  ASSERT_EQ(comment(_), result.front());
  ASSERT_TRUE(result.front().get().find("\\\"name\\\":") != std::string::npos);
  ASSERT_TRUE(result.front().get().find("\\\"display_name\\\":") !=
              std::string::npos);

  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma_engine, switch_code_complete)
{
  code_completer c;

  const auto engines = res("clang", "gcc", "internal", "msvc", "null",
                           "pure_wave", "templight", "wave");

  ASSERT_EQ(c("#msh engine s"), res("how", "witch"));

  ASSERT_EQ(c("#msh engine switch"), engines.prefixed_with(" "));
  ASSERT_EQ(c("#msh engine switch "), engines);
  ASSERT_EQ(c("#msh engine switch c"), res("lang"));
  ASSERT_EQ(c("#msh engine switch cl"), res("ang"));
  ASSERT_EQ(c("#msh engine switch cla"), res("ng"));
  ASSERT_EQ(c("#msh engine switch clan"), res("g"));
  ASSERT_EQ(c("#msh engine switch clang"), res(""));
  ASSERT_EQ(c("#msh engine switch g"), res("cc"));
  ASSERT_EQ(c("#msh engine switch gc"), res("c"));
  ASSERT_EQ(c("#msh engine switch gcc"), res(""));
  ASSERT_EQ(c("#msh engine switch i"), res("nternal"));
  ASSERT_EQ(c("#msh engine switch in"), res("ternal"));
  ASSERT_EQ(c("#msh engine switch int"), res("ernal"));
  ASSERT_EQ(c("#msh engine switch inte"), res("rnal"));
  ASSERT_EQ(c("#msh engine switch inter"), res("nal"));
  ASSERT_EQ(c("#msh engine switch intern"), res("al"));
  ASSERT_EQ(c("#msh engine switch interna"), res("l"));
  ASSERT_EQ(c("#msh engine switch internal"), res(""));
  ASSERT_EQ(c("#msh engine switch m"), res("svc"));
  ASSERT_EQ(c("#msh engine switch ms"), res("vc"));
  ASSERT_EQ(c("#msh engine switch msv"), res("c"));
  ASSERT_EQ(c("#msh engine switch msvc"), res(""));
  ASSERT_EQ(c("#msh engine switch n"), res("ull"));
  ASSERT_EQ(c("#msh engine switch nu"), res("ll"));
  ASSERT_EQ(c("#msh engine switch nul"), res("l"));
  ASSERT_EQ(c("#msh engine switch null"), res(""));
  ASSERT_EQ(c("#msh engine switch p"), res("ure_wave"));
  ASSERT_EQ(c("#msh engine switch pu"), res("re_wave"));
  ASSERT_EQ(c("#msh engine switch pur"), res("e_wave"));
  ASSERT_EQ(c("#msh engine switch pure"), res("_wave"));
  ASSERT_EQ(c("#msh engine switch pure_"), res("wave"));
  ASSERT_EQ(c("#msh engine switch pure_w"), res("ave"));
  ASSERT_EQ(c("#msh engine switch pure_wa"), res("ve"));
  ASSERT_EQ(c("#msh engine switch pure_wav"), res("e"));
  ASSERT_EQ(c("#msh engine switch pure_wave"), res(""));
  ASSERT_EQ(c("#msh engine switch t"), res("emplight"));
  ASSERT_EQ(c("#msh engine switch te"), res("mplight"));
  ASSERT_EQ(c("#msh engine switch tem"), res("plight"));
  ASSERT_EQ(c("#msh engine switch temp"), res("light"));
  ASSERT_EQ(c("#msh engine switch templ"), res("ight"));
  ASSERT_EQ(c("#msh engine switch templi"), res("ght"));
  ASSERT_EQ(c("#msh engine switch templig"), res("ht"));
  ASSERT_EQ(c("#msh engine switch templigh"), res("t"));
  ASSERT_EQ(c("#msh engine switch templight"), res(""));
  ASSERT_EQ(c("#msh engine switch w"), res("ave"));
  ASSERT_EQ(c("#msh engine switch wa"), res("ve"));
  ASSERT_EQ(c("#msh engine switch wav"), res("e"));
  ASSERT_EQ(c("#msh engine switch wave"), res(""));
}
