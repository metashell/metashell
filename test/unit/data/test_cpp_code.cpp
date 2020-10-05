// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/cpp_code.hpp>

#include <gtest/gtest.h>

#include <functional>

using namespace metashell::data;

namespace
{
  std::string fails_with(const std::function<void()>& f_)
  {
    try
    {
      f_();
    }
    catch (const std::runtime_error& e)
    {
      return e.what();
    }
    return std::string();
  }
} // namespace

TEST(cpp_code, test_invalid_markers)
{
  ASSERT_EQ(
      "Marker (\n* __METASHELL_PP_MARKER *\n) not found in preprocessed "
      "output. Does it contain a macro that has been defined?",
      fails_with([] { remove_markers(cpp_code(""), true); }));

  ASSERT_EQ(
      "Marker (\n* __METASHELL_PP_MARKER *\n) found only once in preprocessed "
      "output.",
      fails_with([] {
        remove_markers(
            cpp_code("before\n* __METASHELL_PP_MARKER *\nafter"), true);
      }));

  ASSERT_EQ(
      "Marker (\n* __METASHELL_PP_MARKER *\n) found more than two times in "
      "preprocessed output.",
      fails_with([] {
        remove_markers(cpp_code("before\n* __METASHELL_PP_MARKER *\nx\n* "
                                "__METASHELL_PP_MARKER *\ny\n* "
                                "__METASHELL_PP_MARKER *\nafter"),
                       true);
      }));

  ASSERT_EQ(
      "Marker (\n* __METASHELL_PP_MARKER *\n) found more than two times in "
      "preprocessed output.",
      fails_with([] {
        remove_markers(cpp_code("before\n* __METASHELL_PP_MARKER *\n* "
                                "__METASHELL_PP_MARKER *\n* "
                                "__METASHELL_PP_MARKER *\nafter"),
                       true);
      }));
}

TEST(cpp_code, test_markers)
{
  ASSERT_EQ("", remove_markers(cpp_code("before\n"
                                        "* __METASHELL_PP_MARKER *\n"
                                        "\n"
                                        "* __METASHELL_PP_MARKER *\n"
                                        "after"),
                               true));

  ASSERT_EQ("x", remove_markers(cpp_code("before\n"
                                         "* __METASHELL_PP_MARKER *\n"
                                         "x\n"
                                         "* __METASHELL_PP_MARKER *\n"
                                         "after"),
                                true));

  ASSERT_EQ("x\ny", remove_markers(cpp_code("before\n"
                                            "* __METASHELL_PP_MARKER *\n"
                                            "x\n"
                                            "y\n"
                                            "* __METASHELL_PP_MARKER *\n"
                                            "after"),
                                   true));
}

TEST(cpp_code, test_markers_sharing_new_line)
{
  ASSERT_EQ("", remove_markers(cpp_code("before\n"
                                        "* __METASHELL_PP_MARKER *\n"
                                        "* __METASHELL_PP_MARKER *\n"
                                        "after"),
                               true));
}
