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

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/read_only_path.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/type.hpp>

#include <just/temp.hpp>
#include <just/test.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

using namespace metashell_system_test;

using pattern::_;

namespace
{
  void write_file(const boost::filesystem::path& p_,
                  const std::string& content_)
  {
    const std::string p = p_.string();
    std::ofstream f(p);
    if (f)
    {
      if (!(f << content_))
      {
        throw std::runtime_error("Failed to write into file " + p);
      }
    }
    else
    {
      throw std::runtime_error("Failed to create file " + p);
    }
  }

  bool can_create_file_in(const boost::filesystem::path& p_)
  {
    return !std::ofstream((p_ / "test.txt").string()).fail();
  }
}

JUST_TEST_CASE(test_include_from_cwd)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  write_file(temp / "foobar.hpp", "typedef int foo;\n");

  const auto r = in_directory(temp).run_metashell(
      {command("#include \"foobar.hpp\""), command("foo")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_adding_cwd_to_include_path)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  create_directory(temp / "foo");
  write_file(temp / "foo" / "bar.hpp", "typedef int foo;\n");

  const auto r = in_directory(temp).run_metashell(
      {command("#include <foo/bar.hpp>"), command("foo")}, {"--", "-I."});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_relative_include_directory)
{
  just::temp::directory temp_dir;
  const boost::filesystem::path temp(temp_dir.path());

  create_directory(temp / "foo");
  write_file(temp / "foo" / "bar.hpp", "typedef int foo;\n");

  const auto r = in_directory(temp).run_metashell(
      {command("#include <bar.hpp>"), command("foo")}, {"--", "-Ifoo"});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_metashell_definition_succeeds_when_cwd_is_not_writable)
{
  const read_only_path read_only;

  JUST_ASSERT(!can_create_file_in(read_only.path()));

  const auto r =
      in_directory(read_only.path())
          .run_metashell({command("typedef int foo;"), command("foo")});

  auto i = r.begin();
  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
  ++i;
  JUST_ASSERT(i == r.end());
}
