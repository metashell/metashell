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

#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/util.hpp>

#include <gtest/gtest.h>

#include <just/temp.hpp>

#include <boost/filesystem/path.hpp>

#include <fstream>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  json_string ft_of_include(const std::string& tmp_dir_path_,
                            const std::string& a_,
                            const std::string& b_)
  {
    metashell_instance mi(with_quoteincludes({"--"}, {tmp_dir_path_}));
    mi.command("#define A " + a_);
    mi.command("#define B " + b_);
    mi.command("#msh pdb #include \"test.hpp\"");
    return mi.command("ft").front();
  }

  json_string ft_of_include(const std::string& included_content_)
  {
    just::temp::directory tmp_dir;
    {
      std::ofstream f(
          (boost::filesystem::path(tmp_dir.path()) / "test.hpp").string());
      f << included_content_;
    }

    metashell_instance mi(with_quoteincludes({"--"}, {tmp_dir.path()}));
    mi.command("#msh pdb #include \"test.hpp\"");
    return mi.command("ft").front();
  }
}

TEST(pdb, conditionals_in_trace)
{
  just::temp::directory tmp_dir;
  {
    std::ofstream f(
        (boost::filesystem::path(tmp_dir.path()) / "test.hpp").string());
    f << "#if A\n";
    f << "a\n";
    f << "#elif B\n";
    f << "b\n";
    f << "#else\n";
    f << "c\n";
    f << "#endif\n";
  }

  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 4},
          {frame(type("#if A"), _, _, event_kind::preprocessing_condition), 2, 2},
            {frame(type("A"), _, _, event_kind::macro_expansion), 3, 1},
              {frame(type("1"), _, _, event_kind::rescanning), 4, 1},
                {frame(type("1"), _, _, event_kind::expanded_code), 5, 0},
            {frame(type("true"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("a"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0}
    }),
    ft_of_include(tmp_dir.path(), "1", "1")
  );

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 11},
          {frame(type("#if A"), _, _, event_kind::preprocessing_condition), 2, 2},
            {frame(type("A"), _, _, event_kind::macro_expansion), 3, 1},
              {frame(type("0"), _, _, event_kind::rescanning), 4, 1},
                {frame(type("0"), _, _, event_kind::expanded_code), 5, 0},
            {frame(type("false"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("#elif B"), _, _, event_kind::preprocessing_condition), 2, 2},
            {frame(type("B"), _, _, event_kind::macro_expansion), 3, 1},
              {frame(type("1"), _, _, event_kind::rescanning), 4, 1},
                {frame(type("1"), _, _, event_kind::expanded_code), 5, 0},
            {frame(type("true"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("#line"), _, _, event_kind::generated_token), 2, 0},
          {frame(type(" "), _, _, event_kind::generated_token), 2, 0},
          {frame(type("4"), _, _, event_kind::generated_token), 2, 0},
          {frame(type(" "), _, _, event_kind::generated_token), 2, 0},
          {frame(_, _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("b"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("#else"), _, _, event_kind::preprocessing_else), 2, 0}
    }),
    ft_of_include(tmp_dir.path(), "0", "1")
  );

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 11},
          {frame(type("#if A"), _, _, event_kind::preprocessing_condition), 2, 2},
            {frame(type("A"), _, _, event_kind::macro_expansion), 3, 1},
              {frame(type("0"), _, _, event_kind::rescanning), 4, 1},
                {frame(type("0"), _, _, event_kind::expanded_code), 5, 0},
            {frame(type("false"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("#elif B"), _, _, event_kind::preprocessing_condition), 2, 2},
            {frame(type("B"), _, _, event_kind::macro_expansion), 3, 1},
              {frame(type("0"), _, _, event_kind::rescanning), 4, 1},
                {frame(type("0"), _, _, event_kind::expanded_code), 5, 0},
            {frame(type("false"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("#line"), _, _, event_kind::generated_token), 2, 0},
          {frame(type(" "), _, _, event_kind::generated_token), 2, 0},
          {frame(type("6"), _, _, event_kind::generated_token), 2, 0},
          {frame(type(" "), _, _, event_kind::generated_token), 2, 0},
          {frame(_, _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("c"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("#endif"), _, _, event_kind::preprocessing_endif), 2, 0}
    }),
    ft_of_include(tmp_dir.path(), "0", "0")
  );

  // clang-format on
}

TEST(pdb, ifdef_in_trace)
{
  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 1},
          {frame(type("#ifdef X"), _, _, event_kind::preprocessing_condition), 2, 1},
            {frame(type("false"), _, _, event_kind::preprocessing_condition_result), 3, 0}
    }),
    ft_of_include("#ifdef X\nx\n#endif\n")
  );

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 5},
          {frame(type("#ifdef __DATE__"), _, _, event_kind::preprocessing_condition), 2, 1},
            {frame(type("true"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("x"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("#endif"), _, _, event_kind::preprocessing_endif), 2, 0}
    }),
    ft_of_include("#ifdef __DATE__\nx\n#endif\n")
  );

  // clang-format on
}

TEST(pdb, ifndef_in_trace)
{
  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 1},
          {frame(type("#ifndef __DATE__"), _, _, event_kind::preprocessing_condition), 2, 1},
            {frame(type("true"), _, _, event_kind::preprocessing_condition_result), 3, 0}
    }),
    ft_of_include("#ifndef __DATE__\nx\n#endif\n")
  );

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 1},
        {frame(_, _, _, event_kind::quote_include), 1, 5},
          {frame(type("#ifndef X"), _, _, event_kind::preprocessing_condition), 2, 1},
            {frame(type("false"), _, _, event_kind::preprocessing_condition_result), 3, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("x"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
          {frame(type("#endif"), _, _, event_kind::preprocessing_endif), 2, 0}
    }),
    ft_of_include("#ifndef X\nx\n#endif\n")
  );

  // clang-format on
}
