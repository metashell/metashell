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

#include <metashell/header_file_environment.hpp>
#include <metashell/in_memory_environment.hpp>

#include <metashell/config.hpp>

#include <just/test.hpp>
#include <just/temp.hpp>

#include "test_shell.hpp"
#include "argv0.hpp"

#include <algorithm>
#include <fstream>

using namespace metashell;

namespace
{
  void test_append_text_to_environment(environment& env_)
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

JUST_TEST_CASE(test_empty_in_memory_environment_is_empty)
{
  const config cfg = empty_config(argv0::get());

  in_memory_environment env("foo", cfg);

  JUST_ASSERT_EQUAL("", env.get_all());
}

JUST_TEST_CASE(test_append_text_to_in_memory_environment)
{
  const config cfg = empty_config(argv0::get());

  in_memory_environment env("foo", cfg);

  test_append_text_to_environment(env);
}

JUST_TEST_CASE(test_empty_header_file_environment_is_empty)
{
  config cfg = empty_config(argv0::get());
  cfg.use_precompiled_headers = false;

  header_file_environment env(cfg);

  JUST_ASSERT_EQUAL("", env.get_all());
}

JUST_TEST_CASE(test_append_text_to_header_file_environment)
{
  config cfg = empty_config(argv0::get());
  cfg.use_precompiled_headers = false;

  header_file_environment env(cfg);

  test_append_text_to_environment(env);
}

JUST_TEST_CASE(test_reload_environment_rebuilds_the_environment_object)
{
  test_shell sh;
  const environment* old_env_ptr = &sh.env();

  sh.line_available("#msh environment reload");

  JUST_ASSERT_NOT_EQUAL(old_env_ptr, &sh.env());
}

JUST_TEST_CASE(test_template_depth_is_set_by_the_environment)
{
  config cfg;
  cfg.max_template_depth = 13;

  in_memory_environment e(".", cfg);

  const auto& as = e.clang_arguments();

  JUST_ASSERT(
    std::find(as.begin(), as.end(), "-ftemplate-depth=13") != as.end()
  );
}

JUST_TEST_CASE(test_invalid_environment_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh environment foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_invalid_environment_pop_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh environment push");
  sh.line_available("#msh environment pop foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_invalid_environment_push_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh environment push foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_invalid_environment_reload_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh environment reload foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_invalid_environment_stack_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh environment stack foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_invalid_environment_reset_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh environment reset foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_invalid_quit_command_displays_an_error)
{
  test_shell sh;

  sh.line_available("#msh quit foo");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(
  test_environment_save_displays_an_error_when_not_enabled_in_config
)
{
  test_shell sh;

  sh.line_available("#msh environment save");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(
  test_environment_save_saves_the_environment_when_enabled_in_config
)
{
  just::temp::directory d;
  const std::string fn = d.path() + "/test.hpp";

  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.saving_enabled = true;
  test_shell sh(cfg, 80);

  sh.line_available("#msh environment save " + fn);

  JUST_ASSERT_EQUAL("", sh.error());
  JUST_ASSERT(file_exists(fn));
}

JUST_TEST_CASE(
  test_environment_save_displays_an_error_when_filename_is_missing
)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.saving_enabled = true;
  test_shell sh(cfg, 80);

  sh.line_available("#msh environment save    ");

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

JUST_TEST_CASE(
  test_environment_save_displays_an_error_when_io_error_happens
)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.saving_enabled = true;
  test_shell sh(cfg, 80);

#ifdef _WIN32
  sh.line_available("#msh environment save /foo *? bar");
#else
  sh.line_available("#msh environment save /");
#endif

  JUST_ASSERT_NOT_EQUAL("", sh.error());
}

