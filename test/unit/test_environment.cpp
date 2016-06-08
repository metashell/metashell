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

#include <metashell/engine_constant.hpp>
#include <metashell/header_file_environment.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/shell.hpp>

#include <metashell/data/config.hpp>

#include <just/temp.hpp>
#include <just/test.hpp>

#include "test_config.hpp"

#include <algorithm>
#include <fstream>

using namespace metashell;

namespace
{
  void test_append_text_to_environment(iface::environment& env_)
  {
    env_.append("#include <foo/bar.hpp>\n");

    JUST_ASSERT_EQUAL("#include <foo/bar.hpp>\n", env_.get_all());
  }

  bool file_exists(const std::string& path_)
  {
    std::ifstream f(path_.c_str());
    return !(f.fail() || f.bad());
  }
}

JUST_TEST_CASE(test_empty_header_file_environment_is_empty)
{
  data::config cfg{};
  cfg.use_precompiled_headers = false;

  auto engine = create_failing_engine();
  header_file_environment env(*engine, cfg, "", "");

  JUST_ASSERT_EQUAL("", env.get_all());
}

JUST_TEST_CASE(test_append_text_to_header_file_environment)
{
  data::config cfg{};
  cfg.use_precompiled_headers = false;

  auto engine = create_failing_engine();
  header_file_environment env(*engine, cfg, "", "");

  test_append_text_to_environment(env);
}

JUST_TEST_CASE(test_reload_environment_rebuilds_the_environment_object)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  const iface::environment* old_env_ptr = &sh.env();

  sh.line_available("#msh environment reload", d);

  JUST_ASSERT_NOT_EQUAL(old_env_ptr, &sh.env());
}

JUST_TEST_CASE(test_invalid_environment_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_invalid_environment_pop_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment push", d);
  sh.line_available("#msh environment pop foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_invalid_environment_push_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment push foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_invalid_environment_reload_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment reload foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_invalid_environment_stack_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment stack foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_invalid_environment_reset_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment reset foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_invalid_quit_command_displays_an_error)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh quit foo", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(
    test_environment_save_displays_an_error_when_not_enabled_in_config)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("#msh environment save", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(
    test_environment_save_saves_the_environment_when_enabled_in_config)
{
  just::temp::directory d;
  const std::string fn = d.path() + "/test.hpp";

  data::config cfg{};
  cfg.saving_enabled = true;
  in_memory_displayer disp;
  shell sh(cfg, "", "", "", create_failing_engine());

  sh.line_available("#msh environment save " + fn, disp);

  JUST_ASSERT_EMPTY_CONTAINER(disp.errors());
  JUST_ASSERT(file_exists(fn));
}

JUST_TEST_CASE(test_environment_save_displays_an_error_when_filename_is_missing)
{
  data::config cfg{};
  cfg.saving_enabled = true;
  in_memory_displayer d;
  shell sh(cfg, "", "", "", create_failing_engine());

  sh.line_available("#msh environment save    ", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_environment_save_displays_an_error_when_io_error_happens)
{
  data::config cfg{};
  cfg.saving_enabled = true;
  in_memory_displayer d;
  shell sh(cfg, "", "", "", create_failing_engine());

#ifdef _WIN32
  sh.line_available("#msh environment save /foo *? bar", d);
#else
  sh.line_available("#msh environment save /", d);
#endif

  JUST_ASSERT(!d.errors().empty());
}
