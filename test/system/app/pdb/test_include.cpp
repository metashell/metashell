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

#include <boost/filesystem.hpp>

#include <fstream>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  template <event_kind Kind>
  std::string include_arg()
  {
    return Kind == event_kind::sys_include ? "<test.hpp>" : "\"test.hpp\"";
  }

  // This is a workaround for Boost.Wave: it returns path on Windows as
  // C:/.... instead of C:\.....
  // TODO: once other preprocessors (returning the path as C:\...) are also
  // supported, a more flexible solution is need here.
  std::string workaround_for_wave_on_windows(std::string path_)
  {
#ifdef _WIN32
    if (path_.size() > 2 && path_[1] == ':' && path_[2] == '\\')
    {
      path_.replace(2, 1, "/");
    }
#endif
    return path_;
  }

  std::string test_hpp_path(const boost::filesystem::path& tmp_dir_path_)
  {
    return workaround_for_wave_on_windows(
        (boost::filesystem::canonical(tmp_dir_path_) / "test.hpp").string());
  }

  template <event_kind Kind>
  json_string ft_of_include(const std::string& tmp_dir_path_)
  {
    auto with_includes = Kind == event_kind::sys_include ? &with_sysincludes :
                                                           &with_quoteincludes;

    metashell_instance mi(with_includes({"--"}, {tmp_dir_path_}));
    mi.command("#msh pdb #include " + include_arg<Kind>());
    return mi.command("ft").front();
  }

  template <event_kind Kind>
  call_graph expected_call_graph(const boost::filesystem::path& tmp_dir_path_)
  {
    const std::string test_hpp = test_hpp_path(tmp_dir_path_);

    // clang-format off

    const file_location l1(_, 1, _);

    return
      call_graph({
        {frame(type("#include " + include_arg<Kind>())), 0, 3},
        {frame(type("#include"), _, _, event_kind::skipped_token), 1, 0},
        {frame(type("\\n"), _, _, event_kind::skipped_token), 1, 0},
        {frame(type(test_hpp), _, _, Kind), 1, 8},
          {frame(type("#line"), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type(" "), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type("1"), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type(" "), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type(c_string_literal(test_hpp)), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type("hello"), l1, l1, event_kind::generated_token), 2, 0},
          {frame(type("\\n"), l1, l1, event_kind::generated_token), 2, 0}
      });

    // clang-format on
  }
}

TEST(pdb, tokens_from_include)
{
  just::temp::directory tmp_dir;
  {
    std::ofstream f(test_hpp_path(tmp_dir.path()));
    f << "hello\n";
  }

  ASSERT_EQ(expected_call_graph<event_kind::sys_include>(tmp_dir.path()),
            ft_of_include<event_kind::sys_include>(tmp_dir.path()));

  ASSERT_EQ(expected_call_graph<event_kind::quote_include>(tmp_dir.path()),
            ft_of_include<event_kind::quote_include>(tmp_dir.path()));
}

TEST(pdb, error_directive_in_include)
{
  just::temp::directory tmp_dir;
  const auto test_hpp = test_hpp_path(tmp_dir.path());
  {
    std::ofstream f(test_hpp);
    f << "hello\n#error test error\n";
  }

  // clang-format off

  ASSERT_EQ(
    call_graph({
      {frame(type("#include \"test.hpp\"")), 0, 3},
      {frame(type("#include"), _, _, event_kind::skipped_token), 1, 0},
      {frame(type("\\n"), _, _, event_kind::skipped_token), 1, 0},
      {frame(type(test_hpp), _, _, event_kind::quote_include), 1, 9},
        {frame(type("#line"), _, _, event_kind::generated_token), 2, 0},
        {frame(type(" "), _, _, event_kind::generated_token), 2, 0},
        {frame(type("1"), _, _, event_kind::generated_token), 2, 0},
        {frame(type(" "), _, _, event_kind::generated_token), 2, 0},
        {frame(type(c_string_literal(test_hpp)), _, _, event_kind::generated_token), 2, 0},
        {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
        {frame(type("hello"), _, _, event_kind::generated_token), 2, 0},
        {frame(type("\\n"), _, _, event_kind::generated_token), 2, 0},
        {frame(type("#error test error"), _, _, event_kind::error_directive), 2, 0}
    }),
    ft_of_include<event_kind::quote_include>(tmp_dir.path())
  );

  // clang-format on
}
