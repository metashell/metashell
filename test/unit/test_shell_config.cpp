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

#include <metashell/core/default_environment_detector.hpp>
#include <metashell/core/engine_constant.hpp>
#include <metashell/core/header_file_environment.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/shell.hpp>

#include <metashell/data/config.hpp>

#include <gtest/gtest.h>

#include <sstream>

TEST(shell_config, verbose_mode_is_disabled_from_config)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.verbose = false;

  metashell::shell sh(cfg, "", "", "", metashell::create_failing_engine());

  ASSERT_FALSE(sh.verbose());
}

TEST(shell_config, verbose_mode_is_enabled_from_config)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.verbose = true;

  metashell::shell sh(cfg, "", "", "", metashell::create_failing_engine());

  ASSERT_TRUE(sh.verbose());
}

TEST(shell_config, verbose_mode_is_enabled_at_runtime)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.verbose = false;

  metashell::shell sh(cfg, "", "", "", metashell::create_failing_engine());
  sh.verbose(true);

  ASSERT_TRUE(sh.verbose());
}

TEST(shell_config, verbose_mode_is_disabled_at_runtime)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.verbose = true;

  metashell::shell sh(cfg, "", "", "", metashell::create_failing_engine());
  sh.verbose(false);

  ASSERT_FALSE(sh.verbose());
}

TEST(shell_config, new_shell_not_stopped)
{
  metashell::shell sh(
      metashell::test_config(), "", "", "", metashell::create_failing_engine());

  ASSERT_FALSE(sh.stopped());
}

TEST(shell_config, shell_stopped_after_stop)
{
  metashell::shell sh(
      metashell::test_config(), "", "", "", metashell::create_failing_engine());
  sh.stop();

  ASSERT_TRUE(sh.stopped());
}

TEST(shell_config, shell_not_using_precompiled_headers)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.active_shell_config().use_precompiled_headers = false;

  metashell::shell sh(cfg, "", "", "", metashell::create_failing_engine());

  ASSERT_FALSE(sh.using_precompiled_headers());
}

TEST(shell_config, starting_shell_in_metaprogramming_mode)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.active_shell_config().preprocessor_mode = false;

  const metashell::shell sh(
      cfg, "", "", "", metashell::create_failing_engine());

  ASSERT_FALSE(sh.echo());
  ASSERT_TRUE(sh.show_cpp_errors());
  ASSERT_TRUE(sh.evaluate_metaprograms());
}

TEST(shell_config, starting_shell_in_preprocessor_mode)
{
  metashell::data::config cfg;
  cfg.push_back(metashell::data::shell_config());
  cfg.active_shell_config().preprocessor_mode = true;

  const metashell::shell sh(
      cfg, "", "", "", metashell::create_failing_engine());

  ASSERT_TRUE(sh.echo());
  ASSERT_FALSE(sh.show_cpp_errors());
  ASSERT_FALSE(sh.evaluate_metaprograms());
}
