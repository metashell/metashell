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

#include <metashell/main_shell/shell.hpp>

#include <metashell/engine/constant/builder.hpp>

#include <metashell/core/default_environment_detector.hpp>
#include <metashell/core/header_file_environment.hpp>
#include <metashell/core/in_memory_displayer.hpp>

#include <metashell/data/config.hpp>

#include "test_config.hpp"

#include <gtest/gtest.h>

#include <sstream>

namespace
{
  metashell::data::shell_config default_config()
  {
    return metashell::data::shell_config(
        metashell::data::shell_config_name("test"),
        metashell::data::shell_config_data());
  }
} // namespace

TEST(shell_config, verbose_mode_is_disabled_from_config)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.verbose = false;

  metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());

  ASSERT_FALSE(sh.enabled(metashell::data::shell_flag::verbose));
}

TEST(shell_config, verbose_mode_is_enabled_from_config)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.verbose = true;

  metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());

  ASSERT_TRUE(sh.enabled(metashell::data::shell_flag::verbose));
}

TEST(shell_config, verbose_mode_is_enabled_at_runtime)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.verbose = false;

  metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());
  sh.enabled(metashell::data::shell_flag::verbose, true);

  ASSERT_TRUE(sh.enabled(metashell::data::shell_flag::verbose));
}

TEST(shell_config, verbose_mode_is_disabled_at_runtime)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.verbose = true;

  metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());
  sh.enabled(metashell::data::shell_flag::verbose, false);

  ASSERT_FALSE(sh.enabled(metashell::data::shell_flag::verbose));
}

TEST(shell_config, new_shell_not_stopped)
{
  metashell::main_shell::shell sh(
      metashell::test_config(), "", "",
      metashell::engine::constant::create_failing());

  ASSERT_FALSE(sh.stopped());
}

TEST(shell_config, shell_stopped_after_stop)
{
  metashell::main_shell::shell sh(
      metashell::test_config(), "", "",
      metashell::engine::constant::create_failing());
  sh.stop();

  ASSERT_TRUE(sh.stopped());
}

TEST(shell_config, shell_not_using_precompiled_headers)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.active_shell_config().use_precompiled_headers = false;

  metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());

  ASSERT_FALSE(
      sh.enabled(metashell::data::shell_flag::use_precompiled_headers));
}

TEST(shell_config, starting_shell_in_metaprogramming_mode)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.active_shell_config().preprocessor_mode = false;

  const metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());

  ASSERT_FALSE(sh.enabled(metashell::data::shell_flag::echo));
  ASSERT_TRUE(sh.enabled(metashell::data::shell_flag::show_cpp_errors));
  ASSERT_TRUE(sh.enabled(metashell::data::shell_flag::evaluate_metaprograms));
}

TEST(shell_config, starting_shell_in_preprocessor_mode)
{
  metashell::data::config cfg;
  cfg.push_back(default_config());
  cfg.active_shell_config().preprocessor_mode = true;

  const metashell::main_shell::shell sh(
      cfg, "", "", metashell::engine::constant::create_failing());

  ASSERT_TRUE(sh.enabled(metashell::data::shell_flag::echo));
  ASSERT_FALSE(sh.enabled(metashell::data::shell_flag::show_cpp_errors));
  ASSERT_FALSE(sh.enabled(metashell::data::shell_flag::evaluate_metaprograms));
}
