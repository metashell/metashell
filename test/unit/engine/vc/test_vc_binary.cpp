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

#include <metashell/engine/vc/binary.hpp>

#include <metashell/data/exception.hpp>

#include <gtest/gtest.h>

#include <map>

namespace
{
  std::pair<std::string, std::string> report_for(const std::string& err_)
  {
    const metashell::data::exit_code_t e{1};
    return {metashell::engine::vc::error_report_on_stdout({e, err_, ""}),
            metashell::engine::vc::error_report_on_stderr(
                {e, "", "Compiler info\nin multiple lines\n\n" + err_})};
  }

  std::pair<std::string, std::string> expect(const std::string& s_)
  {
    return {s_, s_};
  }
}

TEST(test_vc_binary, test_vc_error_report)
{
  ASSERT_EQ(expect(""), report_for(""));
  ASSERT_EQ(expect(""), report_for("test.cpp"));
  ASSERT_EQ(expect(""), report_for("test.cpp\n"));
  ASSERT_EQ(
      expect("error C1234: this is a problem.\n"),
      report_for("test.cpp\ntest.cpp(1): error C1234: this is a problem.\n"));
  ASSERT_EQ(
      expect("error C1234: this is a problem.\n"),
      report_for(
          "test.cpp\nC:\\test.cpp(1): error C1234: this is a problem.\n"));
  ASSERT_EQ(expect("error C1234: this is a problem.\n"),
            report_for("test.cpp\nC:\\test.cpp\\test.cpp(1): error C1234: this "
                       "is a problem.\n"));
}

TEST(test_vc_binary, test_vc_error_report_not_supported)
{
  ASSERT_THROW(report_for("foo.cpp\nbar.cpp\n"), metashell::data::exception);
  ASSERT_THROW(
      report_for("foo.cpp\nfoo.cpp: error C1234: this is a problem.\n"),
      metashell::data::exception);
  ASSERT_THROW(
      report_for("foo.cpp\nbar.cpp(1): error C1234: this is a problem.\n"),
      metashell::data::exception);
  ASSERT_THROW(
      report_for("bar.cpp\nfoobar.cpp(1): error C1234: this is a problem.\n"),
      metashell::data::exception);
}
