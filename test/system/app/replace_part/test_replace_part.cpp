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

#include "replace_part.hpp"

#include <gtest/gtest.h>

#include <just/file.hpp>
#include <just/temp.hpp>

#include <cassert>
#include <fstream>

namespace
{
  std::string read(metashell::process::input_file& f_)
  {
    std::string result;
    read_all(std::tie(f_, result));
    return result;
  }

  std::string replace(const std::string& input_,
                      const std::string& marker_,
                      const std::string& replace_with_)
  {
    just::temp::directory tmp;
    const boost::filesystem::path tmpdir = tmp.path();
    const boost::filesystem::path input = tmpdir / "input";
    const boost::filesystem::path replace_with = tmpdir / "replace_with";
    const boost::filesystem::path output = tmpdir / "output";

    just::file::write(input.string(), input_);
    just::file::write(replace_with.string(), replace_with_);

    metashell::process::execution e =
        replace_part::run({"-i", input.string(), "-o", output.string(), "-m",
                           marker_, "-r", replace_with.string()});

    const metashell::data::exit_code_t ecode = e.wait();
    const std::string out = read(e.standard_output());
    const std::string err = read(e.standard_error());

    assert(err.empty());
    assert(out.empty());
    assert(ecode == metashell::data::exit_code_t(0));

    return just::file::read<std::string>(output.string());
  }

  metashell::data::exit_code_t run(const std::vector<std::string>& args_)
  {
    metashell::process::execution e = replace_part::run(args_);
    return e.wait();
  }
}

TEST(replace_part, invalid_args)
{
  just::temp::directory tmp;
  const std::string input =
      (boost::filesystem::path(tmp.path()) / "input").string();
  just::file::write(input, "");

  ASSERT_NE(metashell::data::exit_code_t(0), run({}));
  ASSERT_NE(metashell::data::exit_code_t(0), run({"-i", input}));
  ASSERT_NE(metashell::data::exit_code_t(0), run({"-i", input, "-r", input}));
  ASSERT_NE(metashell::data::exit_code_t(0), run({"-i", input, "-m", "qwe"}));
  ASSERT_NE(metashell::data::exit_code_t(0), run({"-r", "asd", "-m", "qwe"}));
  ASSERT_EQ(metashell::data::exit_code_t(0),
            run({"-i", input, "-r", input, "-m", "qwe"}));
}

TEST(replace_part, no_replacing)
{
  ASSERT_EQ("", replace("", "NOT THERE", "x"));

  ASSERT_EQ("hello", replace("hello", "NOT THERE", "x"));

  ASSERT_EQ("hello\nworld\n", replace("hello\nworld\n", "NOT THERE", "x"));
  ASSERT_EQ("hello\nworld", replace("hello\nworld", "NOT THERE", "x"));
  ASSERT_EQ("hello\rworld", replace("hello\rworld", "NOT THERE", "x"));
  ASSERT_EQ("hello\r\nworld", replace("hello\r\nworld", "NOT THERE", "x"));
  ASSERT_EQ("hello\rworld\n", replace("hello\rworld\n", "NOT THERE", "x"));
  ASSERT_EQ("hello\r\nworld\n", replace("hello\r\nworld\n", "NOT THERE", "x"));

  ASSERT_EQ("\n", replace("\n", "NOT THERE", "x"));
  ASSERT_EQ("\r", replace("\r", "NOT THERE", "x"));
  ASSERT_EQ("\r\n", replace("\r\n", "NOT THERE", "x"));

  ASSERT_EQ("\n\n", replace("\n\n", "NOT THERE", "x"));
  ASSERT_EQ("\r\r", replace("\r\r", "NOT THERE", "x"));
  ASSERT_EQ("\r\n\r\n", replace("\r\n\r\n", "NOT THERE", "x"));

  ASSERT_EQ("\n\r\n", replace("\n\r\n", "NOT THERE", "x"));
}

TEST(replace_part, replacing)
{
  ASSERT_EQ("helloworld\n", replace("hello", "hello", "world"));
  ASSERT_EQ("hello\nworld\n", replace("hello\nworld", "hello", "world"));
  ASSERT_EQ("xhelloz\nworld\n", replace("xhelloz\nfoo", "hello", "world"));
  ASSERT_EQ("xhelloz\nworld\nyhellor\nbar",
            replace("xhelloz\nfoo\nyhellor\nbar", "hello", "world"));
  ASSERT_EQ("xhelloz\rworld\nyhellor\rbar",
            replace("xhelloz\rfoo\ryhellor\rbar", "hello", "world"));
  ASSERT_EQ("xhelloz\r\nworld\nyhellor\r\nbar",
            replace("xhelloz\r\nfoo\r\nyhellor\r\nbar", "hello", "world"));
}
