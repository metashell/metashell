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

#include <metashell/system_test/filename_set.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <gtest/gtest.h>
#include <just/temp.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

using namespace metashell::system_test;

namespace
{
  class included_headers
  {
  public:
    boost::filesystem::path relative(const boost::filesystem::path& path_) const
    {
      return _tmp.path() / path_;
    }

    included_headers& write(const boost::filesystem::path& path_,
                            const std::string& content_)
    {
      create_directories(relative(path_).parent_path());
      std::ofstream f(relative(path_).string());
      f << content_ << "\n";
      return *this;
    }

    json_string run(const std::vector<boost::filesystem::path>& includes_ = {},
                    const std::string& args_ = "") const
    {
      metashell_instance mi(with_sysincludes({"--"}, {_tmp.path()}));
      for (const boost::filesystem::path& include : includes_)
      {
        mi.command("#include <" + include.string() + ">");
      }
      const std::vector<json_string> r =
          args_.empty() ? mi.command("#msh included headers") :
                          mi.command("#msh included headers " + args_);

      const auto i =
          std::find_if(r.begin(), r.end(), [](const json_string& s_) {
            return boost::algorithm::starts_with(
                s_.get(), "{\"type\":\"filename_set\",\"filenames\":[");
          });
      if (i == r.end())
      {
        throw std::runtime_error("No filename_set in output.");
      }
      return *i;
    }

    filename_set
    filenames(const std::vector<boost::filesystem::path>& filenames_)
    {
      return filename_set(filenames_ | boost::adaptors::transformed([this](
                                           const boost::filesystem::path& p_) {
                            return boost::filesystem::canonical(
                                this->relative(p_));
                          }));
    }

  private:
    just::temp::directory _tmp;
  };

  template <int N>
  std::string lines_of_comment()
  {
    std::ostringstream s;
    for (int i = 0; i < N; ++i)
    {
      s << "// " << i << "\n";
    }
    return s.str();
  }

  std::string string_literal(const std::string& value_)
  {
    std::string result(value_.size() * 2 + 2, '"');
    std::string::iterator i = result.begin() + 1;
    for (char c : value_)
    {
      switch (c)
      {
      case '\\':
      case '"':
      case '\'':
        *i = '\\';
        ++i;
        *i = c;
        ++i;
        break;
      case '\r':
        *i = '\\';
        ++i;
        *i = 'r';
        ++i;
        break;
      case '\n':
        *i = '\\';
        ++i;
        *i = 'n';
        ++i;
        break;
      default:
        *i = c;
        ++i;
      }
    }
    result.erase(i + 1, result.end());
    return result;
  }

  std::string string_literal(const boost::filesystem::path& value_)
  {
    return string_literal(value_.string());
  }
}

TEST(included_headers, empty_environment_includes_nothing)
{
  ASSERT_EQ(filename_set{}, included_headers().run());
}

TEST(included_headers, included_header_is_listed)
{
  included_headers h;
  h.write("test.hpp", "");

  ASSERT_EQ(h.filenames({"test.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, two_included_headers)
{
  included_headers h;
  h.write("test1.hpp", "");
  h.write("test2.hpp", "");

  ASSERT_EQ(h.filenames({"test1.hpp", "test2.hpp"}),
            h.run({"test1.hpp", "test2.hpp"}));
}

TEST(included_headers, recursive_inclusion)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("test.hpp", "#include <rec.hpp>");

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, content_in_header)
{
  included_headers h;
  h.write("test.hpp", "int c;\n\ndouble d;\n");

  ASSERT_EQ(h.filenames({"test.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, repeated_recursive_inclusion)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("test.hpp", "#include <rec.hpp>\n#include <rec.hpp>");

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, line_directives_are_ignored)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("test.hpp", "#line 1 " + string_literal(h.relative("rec.hpp")));

  ASSERT_EQ(h.filenames({"test.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headeres, simulated_non_existing_header)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("test.hpp", "#line 1 " + string_literal(h.relative("foo.hpp")) +
                          "\n"
                          "#include <rec.hpp>");

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, line_directives_and_recursive_include)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("foo.hpp", "");
  h.write("bar.hpp", "");
  h.write("test.hpp", "#line 1 " + string_literal(h.relative("foo.hpp")) +
                          "\n"
                          "#include <rec.hpp>\n"
                          "#line 1 " +
                          string_literal(h.relative("bar.hpp")));

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headeres, rewinding_current_header)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("test.hpp", "#line 1 " + string_literal(h.relative("foo.hpp")) +
                          "\n"
                          "#line 1 " +
                          string_literal(h.relative("test.hpp")) +
                          "\n"
                          "#include <rec.hpp>");

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, self_inclusion)
{
  included_headers h;
  h.write("rec.hpp",
          "#ifndef FOO\n"
          "#  define FOO\n"
          "#  include <rec.hpp>\n"
          "#endif\n");
  h.write("test.hpp", "#include <rec.hpp>");

  ASSERT_EQ(
      h.filenames({"test.hpp", "rec.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, multiple_blocks_in_same_file)
{
  included_headers h;
  h.write("test.hpp", "int x;\n" + lines_of_comment<50>() + "int y;\n");

  ASSERT_EQ(h.filenames({"test.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, simulated_jump_forward_in_current_header)
{
  included_headers h;
  h.write("rec.hpp", "");
  h.write("test.hpp", "#line 3 " + string_literal(h.relative("test.hpp")) +
                          "\n"
                          "#include <rec.hpp>\n"
                          "#line 1 " +
                          string_literal(h.relative("foo.hpp")) + "\n");

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, simulated_jump_forward_into_including_header)
{
  included_headers h;
  h.write("foo.hpp", "");
  h.write("bar.hpp", "");

  h.write("rec.hpp", "#line 2 " + string_literal(h.relative("test.hpp")) +
                         "\n"
                         "#include <bar.hpp>\n");

  h.write("test.hpp",
          "#include <rec.hpp>\n"
          "#line 1 " +
              string_literal(h.relative("foo.hpp")) + "\n");

  ASSERT_EQ(
      h.filenames({"test.hpp", "rec.hpp", "bar.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, empty_line_after_include)
{
  included_headers h;
  h.write("foo.hpp", "");
  h.write("rec.hpp", "");

  h.write("test.hpp",
          "#include <rec.hpp>\n"
          "\n"
          "#line 1 " +
              string_literal(h.relative("foo.hpp")) + "\n");

  ASSERT_EQ(h.filenames({"test.hpp", "rec.hpp"}), h.run({"test.hpp"}));
}

TEST(included_headers, space_in_path)
{
  included_headers h;
  h.write("foo bar/x.hpp", "");

  ASSERT_EQ(h.filenames({"foo bar/x.hpp"}), h.run({"foo bar/x.hpp"}));
}

TEST(included_headers, additional_included_headers)
{
  included_headers h;
  h.write("a.hpp", "");
  h.write("b.hpp", "");
  h.write("c.hpp", "#include <a.hpp>\n#include <b.hpp>\n");
  h.write("d.hpp", "#include <a.hpp>");

  ASSERT_EQ(
      h.filenames({"c.hpp", "b.hpp"}), h.run({"d.hpp"}, "#include <c.hpp>"));
}

TEST(included_headers, not_included_headers_are_not_listed)
{
  included_headers h;
  h.write("foo.hpp", "");

  h.write("test.hpp",
          "#if 0\n"
          "#include <foo.hpp>\n"
          "#endif\n");

  ASSERT_EQ(h.filenames({"test.hpp"}), h.run({"test.hpp"}));
}
