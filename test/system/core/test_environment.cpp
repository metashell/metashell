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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

#include <just/temp.hpp>

#include <fstream>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  bool file_exists(const std::string& path_)
  {
    std::ifstream f(path_.c_str());
    return !(f.fail() || f.bad());
  }
} // namespace

TEST(environment, invalid_environment_command_displays_an_error)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh environment foo").front());
}

TEST(environment, invalid_environment_pop_command_displays_an_error)
{
  metashell_instance mi;
  mi.command("#msh environment push");
  ASSERT_EQ(error(_), mi.command("#msh environment pop foo").front());
}

TEST(environment, invalid_environment_push_command_displays_an_error)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh environment push foo").front());
}

TEST(environment, invalid_environment_reload_command_displays_an_error)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh environment reload foo").front());
}

TEST(environment, invalid_environment_stack_command_displays_an_error)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh environment stack foo").front());
}

TEST(environment, invalid_environment_reset_command_displays_an_error)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh environment reset foo").front());
}

TEST(environment, invalid_quit_command_displays_an_error)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh quit foo").front());
}

TEST(environment, environment_save_displays_an_error_when_not_enabled_in_config)
{
  metashell_instance mi(
      {"--disable_saving", "--"}, boost::filesystem::path(), false);
  ASSERT_EQ(error(_), mi.command("#msh environment save test.hpp").front());
}

TEST(environment, environment_save_saves_the_environment_when_enabled_in_config)
{
  just::temp::directory d;
  const std::string fn = d.path() + "/test.hpp";

  metashell_instance mi;
  ASSERT_EQ(1u, mi.command("#msh environment save " + fn).size());

  ASSERT_TRUE(file_exists(fn));
}

TEST(environment, save_displays_an_error_when_filename_is_missing)
{
  metashell_instance mi;
  ASSERT_EQ(error(_), mi.command("#msh environment save    ").front());
}

TEST(environment, save_displays_an_error_when_io_error_happens)
{
  metashell_instance mi;

#ifdef _WIN32
  const std::string cmd = "#msh environment save /foo *? bar";
#else
  const std::string cmd = "#msh environment save /";
#endif

  ASSERT_EQ(error(_), mi.command(cmd).front());
}
