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

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <just/file.hpp>
#include <just/temp.hpp>

using namespace metashell::system_test;

namespace
{
  void touch(const boost::filesystem::path& path_)
  {
    just::file::write(path_.string(), "");
  }
}

TEST(pragma_environment, code_complete)
{
  auto res = code_completion_result::create;
  code_completer c;

  ASSERT_EQ(
      c("#msh environmen"), res("t ", "t add", "t pop", "t push", "t reload",
                                "t reset", "t save", "t stack"));
}

TEST(pragma_environment_add, code_complete)
{
  // most of the code completion testing is covered in test_completion.cpp

  code_completer c;

  ASSERT_EQ(c("#msh environment add #ms"), code_completion_result{});
  ASSERT_EQ(c("#msh environment add #pragma metas"), code_completion_result{});
}

TEST(pragma_environment_save, code_complete)
{
  auto res = code_completion_result::create;
  just::temp::directory tmp_dir;
  const boost::filesystem::path tmp{tmp_dir.path()};
  create_directory(tmp / "comp");
  create_directory(tmp / "comp/foo");

  touch(tmp / "comp/hello.txt");
  touch(tmp / "comp/hello.cpp");
  touch(tmp / "comp/c");
  touch(tmp / "comp/cxx");
  touch(tmp / "comp/hpp");
  touch(tmp / "comp/h");
  touch(tmp / "comp/hxx");
  touch(tmp / "comp/hello.c");
  touch(tmp / "comp/hello.cxx");
  touch(tmp / "comp/hello.hpp");
  touch(tmp / "comp/hello.h");
  touch(tmp / "comp/hello.hxx");
  touch(tmp / "comp/foo.cpp");
  touch(tmp / "comp/foo/CPP");
  touch(tmp / "comp/foo/C");
  touch(tmp / "comp/foo/CXX");
  touch(tmp / "comp/foo/HPP");
  touch(tmp / "comp/foo/H");
  touch(tmp / "comp/foo/HXX");
  touch(tmp / "comp/foo/hello.CPP");
  touch(tmp / "comp/foo/hello.C");
  touch(tmp / "comp/foo/hello.CXX");
  touch(tmp / "comp/foo/hello.HPP");
  touch(tmp / "comp/foo/hello.H");
  touch(tmp / "comp/foo/hello.HXX");

  code_completer c{
      std::string{}, metashell::data::command_line_argument_list{}, tmp};

  ASSERT_EQ(c("#msh environment r"), res("eload", "eset"));
  ASSERT_EQ(c("#msh environment s"), res("ave", "tack"));

  ASSERT_EQ(c("#msh environment save"),
            res(" comp/foo/", " comp/foo.cpp", " comp/hello.c",
                " comp/hello.cpp", " comp/hello.cxx", " comp/hello.h",
                " comp/hello.hpp", " comp/hello.hxx"));

  ASSERT_EQ(c("#msh environment save "),
            res("comp/foo/", "comp/foo.cpp", "comp/hello.c", "comp/hello.cpp",
                "comp/hello.cxx", "comp/hello.h", "comp/hello.hpp",
                "comp/hello.hxx"));

  ASSERT_EQ(c("#msh environment save co"),
            res("mp/foo/", "mp/foo.cpp", "mp/hello.c", "mp/hello.cpp",
                "mp/hello.cxx", "mp/hello.h", "mp/hello.hpp", "mp/hello.hxx"));

  ASSERT_EQ(c("#msh environment save comp"),
            res("/foo/", "/foo.cpp", "/hello.c", "/hello.cpp", "/hello.cxx",
                "/hello.h", "/hello.hpp", "/hello.hxx"));

  ASSERT_EQ(c("#msh environment save comp/"),
            res("foo/", "foo.cpp", "hello.cpp", "hello.c", "hello.cxx",
                "hello.hpp", "hello.h", "hello.hxx"));

  ASSERT_EQ(c("#msh environment save comp/foo"),
            res(".cpp", "/hello.CPP", "/hello.C", "/hello.CXX", "/hello.HPP",
                "/hello.H", "/hello.HXX"));

  ASSERT_EQ(c("#msh environment save comp/foo/"),
            res("hello.CPP", "hello.C", "hello.CXX", "hello.HPP", "hello.H",
                "hello.HXX"));
}
