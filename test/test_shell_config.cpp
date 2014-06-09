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

#include "test_shell.hpp"

#include <metashell/config.hpp>

#include <just/test.hpp>

JUST_TEST_CASE(test_verbose_mode_is_disabled_from_config)
{
  metashell::config cfg = metashell::config::empty();
  cfg.verbose = false;

  test_shell sh(cfg, 80);

  JUST_ASSERT(!sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_enabled_from_config)
{
  metashell::config cfg = metashell::config::empty();
  cfg.verbose = true;

  test_shell sh(cfg, 80);

  JUST_ASSERT(sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_enabled_at_runtime)
{
  metashell::config cfg = metashell::config::empty();
  cfg.verbose = false;

  test_shell sh(cfg, 80);
  sh.verbose(true);

  JUST_ASSERT(sh.verbose());
}

JUST_TEST_CASE(test_verbose_mode_is_disabled_at_runtime)
{
  metashell::config cfg = metashell::config::empty();
  cfg.verbose = true;

  test_shell sh(cfg, 80);
  sh.verbose(false);

  JUST_ASSERT(!sh.verbose());
}

