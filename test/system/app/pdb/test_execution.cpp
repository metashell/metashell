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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>

#include <gtest/gtest.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <regex>
#include <stdexcept>

using namespace metashell::system_test;
using pattern::_;

namespace
{
  json_string pdb_c_returns(const std::string& expr_)
  {
    metashell_instance mi;
    mi.command("#define FOO bar");
    mi.command("#msh pdb " + expr_);
    const auto r = mi.command("c");

    if (r.size() == 3 && raw_text("Metaprogram finished") == r[0] &&
        prompt("(pdb)") == r[2])
    {
      return r[1];
    }
    else
    {
      throw std::runtime_error(
          "Invalid response in pdb_c_returns: " +
          boost::algorithm::join(
              r | boost::adaptors::transformed(
                      [](const json_string& s_) { return s_.get(); }),
              "\n"));
    }
  }

  std::string remove_location(const std::string& s_)
  {
    std::smatch base_match;
    if (std::regex_match(
            s_, base_match, std::regex("^[^:]*:[0-9]*:[0-9]*:(.*)")))
    {
      if (base_match.size() == 2)
      {
        return base_match[1].str();
      }
    }
    throw std::runtime_error(
        "String does not start with location information: " + s_);
  }
}

TEST(pdb, empty) { ASSERT_EQ(cpp_code("bar"), pdb_c_returns("FOO")); }

TEST(pdb, dash_argument_parsing)
{
  ASSERT_EQ(cpp_code("-full bar"), pdb_c_returns("-full FOO"));
  ASSERT_EQ(cpp_code("bar"), pdb_c_returns("-profile FOO"));
  ASSERT_EQ(cpp_code("bar"), pdb_c_returns("-nocache FOO"));
}

TEST(pdb, help_evaluate)
{
  metashell_instance mi;
  mi.command("#msh pdb");
  const auto r = mi.command("help evaluate");

  ASSERT_GT(r.size(), 1);
  ASSERT_EQ(raw_text(_), r[0]);
  ASSERT_EQ(raw_text(_), r[1]);

  ASSERT_TRUE(raw_text(r[0]).text().find("-full") == std::string::npos);
  ASSERT_TRUE(raw_text(r[1]).text().find("-full") == std::string::npos);
}

TEST(pdb, error_of_broken_macro_call)
{
  metashell_instance mi;
  mi.command("#define FOO(a, b)");
  mi.command("#define BAR FOO(3)");

  const auto nr = mi.command("#msh pp BAR");
  ASSERT_GT(nr.size(), 1);
  const error normal_error(nr[0]);
  ASSERT_TRUE(normal_error.message_specified());

  mi.command("#msh pdb BAR");
  const auto r = mi.command("continue");

  ASSERT_GT(r.size(), 2);
  const error pdb_error(r[1]);
  ASSERT_TRUE(pdb_error.message_specified());

  ASSERT_EQ(remove_location(normal_error.message()),
            remove_location(pdb_error.message()));
}
