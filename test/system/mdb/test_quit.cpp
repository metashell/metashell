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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/nocaches.hpp>
#include <metashell/system_test/prompt.hpp>

#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>
#include <just/temp.hpp>

#include <fstream>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  template <class String>
  void write_file(const boost::filesystem::path& filename_, String content_)
  {
    std::ofstream f(filename_.string().c_str());
    f << content_;
  }
} // namespace

TEST(quit, when_included_file_changes)
{
  for (const std::string& nocache : nocaches())
  {
    just::temp::directory tmp_dir;
    const boost::filesystem::path tmp(tmp_dir.path());

    write_file(tmp / "test.hpp", "");

    metashell_instance mi({}, tmp);
    mi.command("#include \"test.hpp\"");
    mi.command("#msh mdb" + nocache + " int");

    write_file(tmp / "test.hpp", "foo");

    const std::vector<json_string> q = mi.command("q");

    ASSERT_EQ(error(_), q.front());
    ASSERT_EQ(prompt(">"), q.back());
  }
}
