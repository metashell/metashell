// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/read_only_path.hpp>
#include <metashell/system_test/type.hpp>
#include <metashell/system_test/util.hpp>

#include <gtest/gtest.h>
#include <just/temp.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  bool can_create_file_in(const boost::filesystem::path& p_)
  {
    return !std::ofstream((p_ / "test.txt").string()).fail();
  }
}

TEST(cwd, include_from_cwd)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  write_file(temp / "foobar.hpp", "typedef int foo;\n");

  metashell_instance mi({}, temp);
  mi.command("#include \"foobar.hpp\"");

  ASSERT_EQ(type("int"), mi.command("foo").front());
}

TEST(cwd, adding_cwd_to_include_path)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  create_directory(temp / "foo");
  write_file(temp / "foo" / "bar.hpp", "typedef int foo;\n");

  metashell_instance mi(with_sysincludes({"--"}, {"."}), temp);
  mi.command("#include <foo/bar.hpp>");

  ASSERT_EQ(type("int"), mi.command("foo").front());
}

TEST(cwd, relative_include_directory)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  create_directory(temp / "foo");
  write_file(temp / "foo" / "bar.hpp", "typedef int foo;\n");

  metashell_instance mi(with_sysincludes({"--"}, {"foo"}), temp);
  mi.command("#include <bar.hpp>");

  ASSERT_EQ(type("int"), mi.command("foo").front());
}

TEST(cwd, metashell_definition_succeeds_when_cwd_is_not_writable)
{
  const read_only_path read_only;

  ASSERT_FALSE(can_create_file_in(read_only.path()));

  metashell_instance mi({}, read_only.path());
  mi.command("typedef int foo;");

  ASSERT_EQ(type("int"), mi.command("foo").front());
}
