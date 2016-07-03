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

#include <metashell/system_test/filename_list.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/system_test_config.hpp>
#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

#include <just/temp.hpp>
#include <just/test.hpp>

#include <algorithm>

using namespace metashell::system_test;

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
        ends_with({"metashell", "libcxx"}) || ends_with({"windows_headers"}) ||
        ends_with({"windows_headers", "mingw32"}) ||
        ends_with({"templight", "include"}))
    {
      return true;
    }
    const auto end = system_test_config::metashell_args().end();
    return find_if(
               find(system_test_config::metashell_args().begin(), end, "--"),
               end, [&path_](const std::string& s_) {
                 const auto path = try_to_remove_prefix("-I", s_);
                 return path && *path == path_;
               }) != end;
  }

  filename_list remove_metashell_standard_headers(const filename_list& headers_)
  {
    return filename_list(headers_ | boost::adaptors::filtered([](
                                        const boost::filesystem::path& p_) {
                           return !metashell_standard_header_path(p_);
                         }));
  }

  filename_list
  includes(const std::vector<boost::filesystem::path>& sysincludes_,
           const std::vector<boost::filesystem::path>& quoteincludes_,
           const std::string& type_)
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

    return remove_metashell_standard_headers(
        filename_list(metashell_instance(args)
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
}

JUST_TEST_CASE(test_includes)
{
  just::temp::directory tmp_dir;

  const boost::filesystem::path tmp(tmp_dir.path());

  const boost::filesystem::path a = tmp / "a";
  const boost::filesystem::path b = tmp / "b";

  create_directories(a);
  create_directories(b);

  JUST_ASSERT_EMPTY_CONTAINER(sysincludes({}, {}));
  JUST_ASSERT_EQUAL_CONTAINER({a, b}, sysincludes({a, b}, {}));
  JUST_ASSERT_EQUAL_CONTAINER({}, sysincludes({}, {a, b}));

  JUST_ASSERT_EQUAL_CONTAINER({"."}, quoteincludes({}, {}));
  JUST_ASSERT_EQUAL_CONTAINER({".", a, b}, quoteincludes({a, b}, {}));
  JUST_ASSERT_EQUAL_CONTAINER({".", a, b}, quoteincludes({}, {a, b}));
  JUST_ASSERT_EQUAL_CONTAINER({".", a, b}, quoteincludes({b}, {a}));
}
