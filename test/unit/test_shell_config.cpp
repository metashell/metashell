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
#include "argv0.hpp"

#include "mock_libclang.hpp"

#include <metashell/shell.hpp>
#include <metashell/config.hpp>
#include <metashell/user_config.hpp>
#include <metashell/header_file_environment.hpp>
#include <metashell/in_memory_environment.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/null_libclang.hpp>

#include <just/test.hpp>

#include <sstream>

JUST_TEST_CASE(test_verbose_mode_is_disabled_from_config)
{
  metashell::config cfg;
  cfg.verbose = false;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);

  JUST_ASSERT(!sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_enabled_from_config)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.verbose = true;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);

  JUST_ASSERT(sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_enabled_at_runtime)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.verbose = false;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);
  sh.verbose(true);

  JUST_ASSERT(sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_disabled_at_runtime)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.verbose = true;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);
  sh.verbose(false);

  JUST_ASSERT(!sh.verbose());
}

JUST_TEST_CASE(test_new_shell_not_stopped)
{
  metashell::null_libclang lc;
  metashell::shell sh(metashell::test_config(), lc);

  JUST_ASSERT(!sh.stopped());
}

JUST_TEST_CASE(test_shell_stopped_after_stop)
{
  metashell::null_libclang lc;
  metashell::shell sh(metashell::test_config(), lc);
  sh.stop();

  JUST_ASSERT(sh.stopped());
}

JUST_TEST_CASE(test_shell_not_using_precompiled_headers)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = false;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);

  JUST_ASSERT(!sh.using_precompiled_headers());
}

JUST_TEST_CASE(test_shell_using_precompiled_headers)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = true;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);

  JUST_ASSERT(sh.using_precompiled_headers());
}

JUST_TEST_CASE(test_shell_enabling_using_precompiled_headers)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = false;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);
  sh.using_precompiled_headers(true);

  JUST_ASSERT(sh.using_precompiled_headers());
}

JUST_TEST_CASE(test_shell_disabling_using_precompiled_headers)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = true;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);
  sh.using_precompiled_headers(false);

  JUST_ASSERT(!sh.using_precompiled_headers());
}

JUST_TEST_CASE(
  test_shell_with_enabled_precompiled_headers_uses_header_file_environment
)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = false;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);
  sh.using_precompiled_headers(true);

  JUST_ASSERT(
    dynamic_cast<const metashell::header_file_environment*>(&sh.env())
    != nullptr
  );
}

JUST_TEST_CASE(
  test_shell_with_disabled_precompiled_headers_uses_header_file_environment
)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = true;

  metashell::null_libclang lc;
  metashell::shell sh(cfg, lc);
  sh.using_precompiled_headers(false);

  JUST_ASSERT(
    dynamic_cast<const metashell::header_file_environment*>(&sh.env())
    != nullptr
  );
}

JUST_TEST_CASE(test_shell_enabling_precompiled_headers_keeps_the_environment)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = false;

  metashell::in_memory_displayer d;
  metashell::mock_libclang lc;
  expect_parsing_return_empty(lc);
  metashell::shell sh(cfg, lc);
  sh.store_in_buffer("typedef int foo;\n", d);
  const std::string env_before = sh.env().get_all();

  sh.using_precompiled_headers(true);

  JUST_ASSERT_EQUAL(env_before, sh.env().get_all());
}

JUST_TEST_CASE(test_shell_disabling_precompiled_headers_keeps_the_environment)
{
  metashell::config cfg = metashell::empty_config(argv0::get());
  cfg.use_precompiled_headers = true;

  metashell::in_memory_displayer d;
  metashell::mock_libclang lc;
  expect_parsing_return_empty(lc);
  metashell::shell sh(cfg, lc);
  sh.store_in_buffer("typedef int foo;\n", d);
  const std::string env_before = sh.env().get_all();

  sh.using_precompiled_headers(false);

  JUST_ASSERT_EQUAL(env_before, sh.env().get_all());
}

