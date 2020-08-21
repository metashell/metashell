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

#include <just/file.hpp>
#include <just/temp.hpp>

#include <boost/filesystem.hpp>

using namespace metashell::system_test;

namespace
{
  auto res = code_completion_result::create;
}

TEST(pragma_ls, code_complete)
{
  const std::string prefixes[] = {"", " <list>", "\"list\"", " <>"};

  just::temp::directory tmp;
  const boost::filesystem::path tmp_path = tmp.path();

  create_directories(tmp_path / "foobar");
  just::file::write((tmp_path / "foobar/test.hpp").string(), "");
  just::file::write((tmp_path / "other.hpp").string(), "");

  {
    code_completer c{std::string{}, with_includes({"--"}, {tmp_path})};

    for (const std::string prefix : prefixes)
    {
      ASSERT_EQ(c("#msh ls" + prefix), res(" \"", " <"));
      ASSERT_EQ(c("#msh ls" + prefix + " "), res("\"", "<"));
      ASSERT_EQ(c("#msh ls" + prefix + " <othe"), res("r.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " \"othe"), res("r.hpp\""));

      ASSERT_EQ(c("#msh ls" + prefix + " <foob"), res("ar/test.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " <foobar"), res("/test.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " <foobar/"), res("test.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " <foobar/te"), res("st.hpp>"));

      ASSERT_EQ(c("#msh ls" + prefix + " \"foob"), res("ar/test.hpp\""));
      ASSERT_EQ(c("#msh ls" + prefix + " \"foobar"), res("/test.hpp\""));
      ASSERT_EQ(c("#msh ls" + prefix + " \"foobar/"), res("test.hpp\""));
      ASSERT_EQ(c("#msh ls" + prefix + " \"foobar/te"), res("st.hpp\""));
    }
  }

  just::temp::directory tmp_other;
  const boost::filesystem::path tmp_path_other = tmp_other.path();

  just::file::write((tmp_path_other / "foobar").string(), "");

  {
    code_completer c{
        std::string{}, with_includes({"--"}, {tmp_path, tmp_path_other})};

    for (const std::string prefix : prefixes)
    {
      ASSERT_EQ(c("#msh ls" + prefix + " <foob"), res("ar>", "ar/test.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " <foobar"), res(">", "/test.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " <foobar/"), res("test.hpp>"));
      ASSERT_EQ(c("#msh ls" + prefix + " <foobar/te"), res("st.hpp>"));

      ASSERT_EQ(
          c("#msh ls" + prefix + " \"foob"), res("ar\"", "ar/test.hpp\""));
      ASSERT_EQ(c("#msh ls" + prefix + " \"foobar"), res("\"", "/test.hpp\""));
      ASSERT_EQ(c("#msh ls" + prefix + " \"foobar/"), res("test.hpp\""));
      ASSERT_EQ(c("#msh ls" + prefix + " \"foobar/te"), res("st.hpp\""));
    }
  }
}
