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

#include "test_config.hpp"

#include <metashell/shell.hpp>
#include <metashell/header_file_environment.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/engine_constant.hpp>

#include <metashell/data/config.hpp>

#include <just/test.hpp>

#include <sstream>

JUST_TEST_CASE(test_verbose_mode_is_disabled_from_config)
{
  metashell::data::config cfg;
  cfg.verbose = false;

  metashell::shell sh(cfg, "", "", metashell::create_failing_engine());

  JUST_ASSERT(!sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_enabled_from_config)
{
  metashell::data::config cfg{};
  cfg.verbose = true;

  metashell::shell sh(cfg, "", "", metashell::create_failing_engine());

  JUST_ASSERT(sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_enabled_at_runtime)
{
  metashell::data::config cfg{};
  cfg.verbose = false;

  metashell::shell sh(cfg, "", "", metashell::create_failing_engine());
  sh.verbose(true);

  JUST_ASSERT(sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_disabled_at_runtime)
{
  metashell::data::config cfg{};
  cfg.verbose = true;

  metashell::shell sh(cfg, "", "", metashell::create_failing_engine());
  sh.verbose(false);

  JUST_ASSERT(!sh.verbose());
}

JUST_TEST_CASE(test_new_shell_not_stopped)
{
  metashell::shell sh(
      metashell::test_config(), "", "", metashell::create_failing_engine());

  JUST_ASSERT(!sh.stopped());
}

JUST_TEST_CASE(test_shell_stopped_after_stop)
{
  metashell::shell sh(
      metashell::test_config(), "", "", metashell::create_failing_engine());
  sh.stop();

  JUST_ASSERT(sh.stopped());
}

JUST_TEST_CASE(test_shell_not_using_precompiled_headers)
{
  metashell::data::config cfg{};
  cfg.use_precompiled_headers = false;

  metashell::shell sh(cfg, "", "", metashell::create_failing_engine());

  JUST_ASSERT(!sh.using_precompiled_headers());
}

JUST_TEST_CASE(test_starting_shell_in_metaprogramming_mode)
{
  metashell::data::config cfg;
  cfg.preprocessor_mode = false;

  const metashell::shell sh(cfg, "", "", metashell::create_failing_engine());

  JUST_ASSERT(!sh.echo());
  JUST_ASSERT(sh.show_cpp_errors());
  JUST_ASSERT(sh.evaluate_metaprograms());
}

JUST_TEST_CASE(test_starting_shell_in_preprocessor_mode)
{
  metashell::data::config cfg;
  cfg.preprocessor_mode = true;

  const metashell::shell sh(cfg, "", "", metashell::create_failing_engine());

  JUST_ASSERT(sh.echo());
  JUST_ASSERT(!sh.show_cpp_errors());
  JUST_ASSERT(!sh.evaluate_metaprograms());
}
