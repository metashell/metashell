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
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/system_test_config.hpp>
#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/adaptors.hpp>

#include <gtest/gtest.h>
#include <just/lines.hpp>
#include <just/temp.hpp>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
#include <vector>

using namespace metashell::system_test;

namespace
{
  typedef std::pair<std::vector<boost::filesystem::path>,
                    std::vector<boost::filesystem::path>>
      path_vector_pair;

  std::set<std::string> include_set(const json_string& s_)
  {
    if (cpp_code(s_).code().value())
    {
      const std::string code = *cpp_code(s_).code().value();
      const auto result =
          just::lines::view(code) |
          boost::adaptors::filtered(
              [](const std::string& a_) { return !a_.empty(); }) |
          boost::adaptors::transformed([](const std::string& l_) {
            return remove_prefix("#include ", l_);
          });
      return std::set<std::string>(result.begin(), result.end());
    }
    else
    {
      throw std::runtime_error("No code in " + s_.get());
    }
  }

  boost::filesystem::path resolve_symlink(boost::filesystem::path p_)
  {
    for (int i = 0; i != 1000 && is_symlink(p_); ++i)
    {
      p_ = read_symlink(p_);
    }
    return p_;
  }

  void add_directory(path_vector_pair& out_,
                     const boost::filesystem::path& path_)
  {
    if (is_directory(path_))
    {
      for (boost::filesystem::directory_iterator i(path_), e; i != e; ++i)
      {
        const boost::filesystem::path p = resolve_symlink(i->path());

        if (is_directory(p))
        {
          out_.first.push_back(i->path().filename());
        }
        else if (is_regular_file(p))
        {
          out_.second.push_back(i->path().filename());
        }
      }
    }
  }

  path_vector_pair directories_and_files_coming_from_test_arguments()
  {
    // Metashell's internal files and headers
    path_vector_pair result{{"metashell"}, {"metashell_environment.hpp"}};

    for (const std::string& arg : system_test_config::metashell_args())
    {
      if (const auto path = include_path_addition(arg))
      {
        add_directory(result, *path);
      }
    }

    return result;
  }

  std::set<std::string>
  remove_includes_from(const std::vector<boost::filesystem::path>& paths_,
                       std::set<std::string> set_)
  {
    for (const boost::filesystem::path& p : paths_)
    {
      set_.erase("<" + p.string() + ">");
      set_.erase("\"" + p.string() + "\"");
    }
    return set_;
  }

  class ls_output
  {
  public:
    ls_output(const boost::filesystem::path& cwd_,
              const std::vector<boost::filesystem::path>& sysincludes_,
              const std::vector<boost::filesystem::path>& quoteincludes_,
              const std::string& ls_arg_)
    {
      const std::vector<json_string> r =
          metashell_instance(
              with_quoteincludes(
                  with_sysincludes({"--"}, sysincludes_), quoteincludes_),
              cwd_, true, false)
              .command("#msh ls " + ls_arg_);

      auto i = r.begin();

      const auto ignored = directories_and_files_coming_from_test_arguments();

      if (i != r.end() && comment({paragraph{"Directories:"}}) == *i)
      {
        ++i;
        _directories = remove_includes_from(ignored.first, include_set(*i));
        ++i;
      }

      if (i != r.end() && comment({paragraph{"Header files:"}}) == *i)
      {
        ++i;
        _header_files = remove_includes_from(ignored.second, include_set(*i));
        ++i;
      }
    }

    const std::set<std::string>& directories() const { return _directories; }

    const std::set<std::string>& header_files() const { return _header_files; }
  private:
    std::set<std::string> _directories;
    std::set<std::string> _header_files;
  };

  std::set<std::string>
  include(std::set<std::string> args_ = std::set<std::string>())
  {
    return args_;
  }

  void touch(const boost::filesystem::path& path_)
  {
    std::ofstream f(path_.string());

    if (!f)
    {
      throw std::runtime_error("Failed to touch file " + path_.string());
    }
  }

  template <char Before, char After>
  std::string include_arg(std::vector<std::string> path_elements_)
  {
    using boost::filesystem::path;

    const std::string before(1, Before);

    if (path_elements_.empty())
    {
      return before + After;
    }
    else
    {
      return before +
             accumulate(path_elements_.begin() + 1, path_elements_.end(),
                        path(path_elements_.front()),
                        [](path a_, const path& b_) { return a_ /= b_; })
                 .string() +
             After;
    }
  }

  std::string quote(std::vector<std::string> path_elements_)
  {
    return include_arg<'"', '"'>(move(path_elements_));
  }

  std::string sys(std::vector<std::string> path_elements_)
  {
    return include_arg<'<', '>'>(move(path_elements_));
  }
}

TEST(ls, tests)
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

  ASSERT_EQ(include(), ls_output(cwd, {}, {}, "").directories());

  ASSERT_EQ(include(), ls_output(cwd, {}, {}, "<>").directories());

  ASSERT_EQ(include(), ls_output(cwd, {}, {}, "\"\"").directories());

  ASSERT_EQ(include(), ls_output(cwd, {}, {}, "<> \"\"").directories());

  ASSERT_EQ(std::set<std::string>{sys({"foo.hpp"})},
            ls_output(cwd, {a}, {}, "<>").header_files());

  if (!using_wave())
  {
    ASSERT_EQ(std::set<std::string>{quote({"foo.hpp"})},
              ls_output(cwd, {a}, {}, "\"\"").header_files());
  }

  ASSERT_EQ(
      include({sys({"foo.hpp"})}), ls_output(cwd, {b}, {}, "<>").directories());

  ASSERT_EQ(
      std::set<std::string>{sys({"foo.hpp"})}, empty_sys_a_b.directories());
  ASSERT_EQ(
      std::set<std::string>{sys({"foo.hpp"})}, empty_sys_a_b.header_files());

  ASSERT_EQ(include({sys({"foo.hpp"}), sys({"foo.hpp", "bar.hpp"})}),
            ls_output(cwd, {a, b}, {}, sys({"foo.hpp"})).header_files());
}
