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
  template <event_kind Kind>
  std::string include_arg()
  {
    return Kind == event_kind::sys_include ? "<test.hpp>" : "\"test.hpp\"";
  }

  std::string test_hpp_path(const boost::filesystem::path& tmp_dir_path_)
  {
    return (tmp_dir_path_ / "test.hpp").string();
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
        {frame(type("#include " + include_arg<Kind>())), 0, 1},
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
