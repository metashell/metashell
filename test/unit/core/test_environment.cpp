// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/build_default_pragma_map.hpp>
#include <metashell/core/engine_constant.hpp>
#include <metashell/core/header_file_environment.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/shell.hpp>
#include <metashell/core/type_shell_constant.hpp>

#include <metashell/data/config.hpp>

#include <gtest/gtest.h>
#include <just/temp.hpp>

#include "empty_container.hpp"
#include "test_config.hpp"

#include <algorithm>
#include <fstream>

using namespace metashell;

namespace
{
  void test_append_text_to_environment(iface::environment& env_)
  {
    env_.append(data::cpp_code("#include <foo/bar.hpp>\n"));

    ASSERT_EQ("#include <foo/bar.hpp>\n", env_.get_all());
  }

  bool file_exists(const std::string& path_)
  {
    std::ifstream f(path_.c_str());
    return !(f.fail() || f.bad());
  }

  data::result failing_data()
  {
    return {false, "", "Using failing engine", ""};
  }
}

TEST(environment, empty_header_file_environment_is_empty)
{
  data::shell_config cfg{};
  cfg.use_precompiled_headers = false;

  core::type_shell_constant type_shell(failing_data());
  core::header_file_environment env(&type_shell, cfg, "", "");

  ASSERT_EQ("", env.get_all());
}

TEST(environment, append_text_to_header_file_environment)
{
  data::shell_config cfg{};
  cfg.use_precompiled_headers = false;

  core::type_shell_constant type_shell(failing_data());
  core::header_file_environment env(&type_shell, cfg, "", "");

  test_append_text_to_environment(env);
}

TEST(environment, reload_environment_rebuilds_the_environment_object)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine(),
                 core::build_default_pragma_map(nullptr, "", nullptr));
  const iface::environment* old_env_ptr = &sh.env();

  sh.line_available("#msh environment reload", d);

  ASSERT_NE(old_env_ptr, &sh.env());
}

TEST(environment, invalid_environment_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, invalid_environment_pop_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment push", d);
  sh.line_available("#msh environment pop foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, invalid_environment_push_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment push foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, invalid_environment_reload_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment reload foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, invalid_environment_stack_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment stack foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, invalid_environment_reset_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment reset foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, invalid_quit_command_displays_an_error)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh quit foo", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, environment_save_displays_an_error_when_not_enabled_in_config)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  sh.line_available("#msh environment save", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, environment_save_saves_the_environment_when_enabled_in_config)
{
  just::temp::directory d;
  const std::string fn = d.path() + "/test.hpp";

  data::config cfg;
  cfg.push_back(data::shell_config());
  cfg.saving_enabled = true;
  core::in_memory_displayer disp;
  core::shell sh(cfg, "", "", core::create_failing_engine(),
                 core::build_default_pragma_map(nullptr, "", nullptr));

  sh.line_available("#msh environment save " + fn, disp);

  ASSERT_EQ(empty_container, disp.errors());
  ASSERT_TRUE(file_exists(fn));
}

TEST(environment, save_displays_an_error_when_filename_is_missing)
{
  data::config cfg;
  cfg.push_back(data::shell_config());
  cfg.saving_enabled = true;
  core::in_memory_displayer d;
  core::shell sh(cfg, "", "", core::create_failing_engine());

  sh.line_available("#msh environment save    ", d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(environment, save_displays_an_error_when_io_error_happens)
{
  data::config cfg;
  cfg.push_back(data::shell_config());
  cfg.saving_enabled = true;
  core::in_memory_displayer d;
  core::shell sh(cfg, "", "", core::create_failing_engine());

#ifdef _WIN32
  sh.line_available("#msh environment save /foo *? bar", d);
#else
  sh.line_available("#msh environment save /", d);
#endif

  ASSERT_FALSE(d.errors().empty());
}
