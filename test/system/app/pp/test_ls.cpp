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

#include <metashell_system_test/cpp_code.hpp>
#include <metashell_system_test/comment.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/util.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/filesystem.hpp>

#include <just/test.hpp>
#include <just/temp.hpp>
#include <just/lines.hpp>

#include <fstream>
#include <iterator>
#include <algorithm>

using namespace metashell_system_test;

namespace
{
  std::set<std::string> include_set(const json_string& s_)
  {
    JUST_ASSERT(bool(cpp_code(s_).code().value()));

    const std::string code = *cpp_code(s_).code().value();
    const auto result =
        just::lines::view(code) |
        boost::adaptors::filtered([](const std::string& a_)
                                  {
                                    return !a_.empty();
                                  }) |
        boost::adaptors::transformed([](const std::string& l_)
                                     {
                                       return remove_prefix("#include ", l_);
                                     });
    return std::set<std::string>(result.begin(), result.end());
  }

  class ls_output
  {
  public:
    ls_output(const boost::filesystem::path& cwd_,
              const std::vector<boost::filesystem::path>& sysincludes_,
              const std::vector<boost::filesystem::path>& quoteincludes_,
              const std::string& ls_arg_)
    {
      std::vector<std::string> args{"--", "-nostdinc", "-nostdinc++"};
      for (const boost::filesystem::path& p : sysincludes_)
      {
        args.push_back("-I" + p.string());
      }
      for (const boost::filesystem::path& p : quoteincludes_)
      {
        args.push_back("-iquote");
        args.push_back(p.string());
      }

      const auto r = in_directory(cwd_).run_metashell(
          {command("#msh ls " + ls_arg_)}, args);

      auto i = r.begin() + 1;

      if (i != r.end() && comment({paragraph{"Directories:"}}) == *i)
      {
        ++i;
        _directories = include_set(*i);
        ++i;
      }

      if (i != r.end() && comment({paragraph{"Header files:"}}) == *i)
      {
        ++i;
        _header_files = include_set(*i);
        ++i;
      }
    }

    const std::set<std::string>& directories() const { return _directories; }

    const std::set<std::string>& header_files() const { return _header_files; }
  private:
    std::set<std::string> _directories;
    std::set<std::string> _header_files;
  };

  std::set<std::string> include(std::set<std::string> args_) { return args_; }

  void touch(const boost::filesystem::path& path_)
  {
    std::ofstream f(path_.string());

    if (!f)
    {
      throw std::runtime_error("Failed to touch file " + path_.string());
    }
  }
}

JUST_TEST_CASE(test_ls)
{
  just::temp::directory tmp_dir;

  const boost::filesystem::path tmp(tmp_dir.path());

  const boost::filesystem::path cwd = tmp / "cwd";
  const boost::filesystem::path a = tmp / "a";
  const boost::filesystem::path b = tmp / "b";

  create_directories(cwd);
  create_directories(a);
  touch(a / "foo.hpp");
  create_directories(b);
  create_directories(b / "foo.hpp");
  touch(b / "foo.hpp" / "bar.hpp");

  const ls_output empty_sys_a_b(cwd, {a, b}, {}, "<>");

  JUST_ASSERT_EQUAL_CONTAINER(
      include({}), ls_output(cwd, {}, {}, "").directories());

  JUST_ASSERT_EQUAL_CONTAINER(
      include({}), ls_output(cwd, {}, {}, "<>").directories());

  JUST_ASSERT_EQUAL_CONTAINER(
      include({}), ls_output(cwd, {}, {}, "\"\"").directories());

  JUST_ASSERT_EQUAL_CONTAINER(
      include({}), ls_output(cwd, {}, {}, "<> \"\"").directories());

  JUST_ASSERT_EQUAL_CONTAINER(
      {"<foo.hpp>"}, ls_output(cwd, {a}, {}, "<>").header_files());

  JUST_ASSERT_EQUAL_CONTAINER(
      {"\"foo.hpp\""}, ls_output(cwd, {a}, {}, "\"\"").header_files());

  JUST_ASSERT_EQUAL_CONTAINER(
      include({"<foo.hpp>"}), ls_output(cwd, {b}, {}, "<>").directories());

  JUST_ASSERT_EQUAL_CONTAINER({"<foo.hpp>"}, empty_sys_a_b.directories());
  JUST_ASSERT_EQUAL_CONTAINER({"<foo.hpp>"}, empty_sys_a_b.header_files());

  JUST_ASSERT_EQUAL_CONTAINER(
      include({"<foo.hpp>", "<foo.hpp/bar.hpp>"}),
      ls_output(cwd, {a, b}, {}, "<foo.hpp>").header_files());
}
