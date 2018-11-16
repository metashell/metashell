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
#include <metashell/engine/constant/type_shell.hpp>

#include <metashell/core/header_file_environment.hpp>

#include <metashell/iface/environment.hpp>

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/result.hpp>
#include <metashell/data/shell_config.hpp>

#include <gtest/gtest.h>

#include "test_config.hpp"

using namespace metashell;

namespace
{
  void test_append_text_to_environment(iface::environment& env_)
  {
    env_.append(data::cpp_code("#include <foo/bar.hpp>\n"));

    ASSERT_EQ("#include <foo/bar.hpp>\n", env_.get_all());
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

  engine::constant::type_shell type_shell(failing_data());
  core::header_file_environment env(&type_shell, cfg, "", "");

  ASSERT_EQ("", env.get_all());
}

TEST(environment, append_text_to_header_file_environment)
{
  data::shell_config cfg{};
  cfg.use_precompiled_headers = false;

  engine::constant::type_shell type_shell(failing_data());
  core::header_file_environment env(&type_shell, cfg, "", "");

  test_append_text_to_environment(env);
}

TEST(environment, reload_environment_rebuilds_the_environment_object)
{
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  const iface::environment* old_env_ptr = &sh.env();

  sh.rebuild_environment();

  ASSERT_NE(old_env_ptr, &sh.env());
}
