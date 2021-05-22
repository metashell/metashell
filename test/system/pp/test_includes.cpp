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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/filename_list.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/system_test_config.hpp>
#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

#include <gtest/gtest.h>
#include <just/file.hpp>
#include <just/temp.hpp>

#include <algorithm>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  bool metashell_standard_header_path(const boost::filesystem::path& path_)
  {
    const auto ends_with = [&path_](std::vector<std::string> suffix_) {
      boost::filesystem::path p = path_;
      for (const auto& s : suffix_ | boost::adaptors::reversed)
      {
        if (p.filename() != s)
        {
          return false;
        }
        else
        {
          p = p.parent_path();
        }
      }
      return true;
    };

    if (ends_with({"shell"}) || ends_with({"templight"}) ||
        ends_with({"metashell", "libcxx"}) ||
        ends_with({"templight", "include"}))
    {
      return true;
    }
    const auto end = system_test_config::metashell_args().end();
    return find_if(
               find(system_test_config::metashell_args().begin(), end, "--"),
               end,
               [&path_](const metashell::data::command_line_argument& arg) {
                 const auto path = include_path_addition(arg);
                 return path && *path == path_;
               }) != end;
  }

  filename_list remove_metashell_standard_headers(const filename_list& headers_)
  {
    return filename_list(
        headers_ |
        boost::adaptors::filtered([](const boost::filesystem::path& p_) {
          return !metashell_standard_header_path(p_);
        }));
  }

  filename_list
  includes(const std::vector<boost::filesystem::path>& sysincludes_,
           const std::vector<boost::filesystem::path>& quoteincludes_,
           const std::string& type_)
  {
    return remove_metashell_standard_headers(filename_list(
        metashell_instance(
            with_quoteincludes(
                with_sysincludes({"--"}, sysincludes_), quoteincludes_),
            boost::filesystem::path(), true, false)
            .command("#msh " + type_ + "includes")
            .front()));
  }

  filename_list
  sysincludes(const std::vector<boost::filesystem::path>& sysincludes_,
              const std::vector<boost::filesystem::path>& quoteincludes_)
  {
    return includes(sysincludes_, quoteincludes_, "sys");
  }

  filename_list
  quoteincludes(const std::vector<boost::filesystem::path>& sysincludes_,
                const std::vector<boost::filesystem::path>& quoteincludes_)
  {
    return includes(sysincludes_, quoteincludes_, "quote");
  }
} // namespace

TEST(includes, tests)
{
  typedef std::vector<boost::filesystem::path> pv;

  just::temp::directory tmp_dir;

  const boost::filesystem::path tmp(tmp_dir.path());

  const boost::filesystem::path a = tmp / "a";
  const boost::filesystem::path b = tmp / "b";

  create_directories(a);
  create_directories(b);

  ASSERT_EQ(filename_list{}, sysincludes({}, {}));
  ASSERT_EQ((pv{a, b}), sysincludes({a, b}, {}));
  if (using_wave())
  {
    ASSERT_EQ((pv{a, b}), sysincludes({}, {a, b}));
  }
  else if (!using_msvc())
  {
    ASSERT_EQ(pv{}, sysincludes({}, {a, b}));
  }

  ASSERT_EQ(pv{"."}, quoteincludes({}, {}));
  if (!using_wave())
  {
    ASSERT_EQ((pv{".", a, b}), quoteincludes({a, b}, {}));
  }
  ASSERT_EQ((pv{".", a, b}), quoteincludes({}, {a, b}));
  if (!(using_msvc() || using_wave()))
  {
    ASSERT_EQ((pv{".", a, b}), quoteincludes({b}, {a}));
  }
}

#if !defined(_WIN32) && !defined(__APPLE__)
TEST(includes, sysroot)
{
  just::temp::directory tmp_dir;
  const boost::filesystem::path tmp(tmp_dir.path());

  const auto dir = tmp / "usr" / "local" / "include";
  create_directories(dir);
  just::file::write((dir / "test.hpp").string(), "typedef int x;");

  metashell_instance mi{{"--preprocessor", "--", "--sysroot", tmp.string()}};
  ASSERT_EQ(cpp_code(_), mi.command("#include <test.hpp>").front());
  ASSERT_EQ(comment(_), mi.command("#msh engine switch internal").front());
  ASSERT_EQ(comment(_), mi.command("#msh engine switch pure_wave").front());
}
#endif
