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
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/util.hpp>

#include <metashell/core/rapid_json_writer.hpp>

#include <gtest/gtest.h>

#include <just/file.hpp>
#include <just/temp.hpp>

using namespace metashell::system_test;

namespace
{
  auto res = code_completion_result::create;
}

TEST(shell_pragma_config, listing)
{
  just::temp::directory temp_dir;
  const auto config_path =
      boost::filesystem::path(temp_dir.path()) / "config.json";

  just::file::write(
      config_path.string(),
      array({test_config("foo/bar.cpp"), test_config("foo"),
             test_config("foo/baz.cpp"), test_config("app/sample/a.cpp"),
             test_config("app/sample\\b.cpp"), test_config("app/sample/c.cpp")})
          .get());

  metashell_instance mi({"--load_configs", config_path.string()}, {}, false);

  ASSERT_EQ(comment("   app/\n * default\n   foo\n   foo/"),
            mi.command("#msh config").front());

  ASSERT_EQ(
      comment("   bar.cpp\n   baz.cpp"), mi.command("#msh config foo").front());

  ASSERT_EQ(comment("   sample/"), mi.command("#msh config app").front());

  ASSERT_EQ(comment("   a.cpp\n   b.cpp\n   c.cpp"),
            mi.command("#msh config app/sample").front());

  ASSERT_EQ(comment("   a.cpp\n   b.cpp\n   c.cpp"),
            mi.command("#msh config app\\sample").front());

  mi.command("#msh config load app/sample/a.cpp");

  ASSERT_EQ(comment(" * app/\n   default\n   foo\n   foo/"),
            mi.command("#msh config").front());

  ASSERT_EQ(
      comment("   bar.cpp\n   baz.cpp"), mi.command("#msh config foo").front());

  ASSERT_EQ(comment(" * sample/"), mi.command("#msh config app").front());

  ASSERT_EQ(comment(" * a.cpp\n   b.cpp\n   c.cpp"),
            mi.command("#msh config app/sample").front());

  mi.command("#msh config load foo");

  ASSERT_EQ(comment("   app/\n   default\n * foo\n   foo/"),
            mi.command("#msh config").front());

  ASSERT_EQ(
      comment("   bar.cpp\n   baz.cpp"), mi.command("#msh config foo").front());
}

TEST(pragma_config, code_complete)
{
  just::temp::directory temp_dir;
  const auto config_path =
      boost::filesystem::path(temp_dir.path()) / "config.json";

  just::file::write(
      config_path.string(),
      array({test_config("foo/bar.cpp"), test_config("foo"),
             test_config("foo/baz.cpp"), test_config("app/sample/a.cpp"),
             test_config("app/sample\\b.cpp"), test_config("app/sample/c.cpp"),
             test_config("low"), test_config("show/x"),
             test_config("load/load")})
          .get());

  code_completer c{"", {"--load_configs", config_path.string()}};

  const auto all_config = res("default", "foo/bar.cpp", "foo", "foo/baz.cpp",
                              "app/sample/a.cpp", "app/sample\\b.cpp",
                              "app/sample/c.cpp", "low", "show/x", "load/load");

  ASSERT_EQ(
      c("#msh config"), res(" load", " show") + all_config.prefixed_with(" "));
  ASSERT_EQ(c("#msh config "), res("load", "show") + all_config);
  ASSERT_EQ(c("#msh config lo"), res("ad", "w", "ad/load"));
  ASSERT_EQ(
      c("#msh config load"), res("/load") + all_config.prefixed_with(" "));
  ASSERT_EQ(c("#msh config load/"), res("load"));
  ASSERT_EQ(c("#msh config load "), all_config);
  ASSERT_EQ(c("#msh config show"), res("/x") + all_config.prefixed_with(" "));
  ASSERT_EQ(c("#msh config show/"), res("x"));
  ASSERT_EQ(c("#msh config show "), all_config);
  ASSERT_EQ(c("#msh config default"), res(""));
}
