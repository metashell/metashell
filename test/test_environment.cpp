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

#include "test_shell.hpp"

using namespace metashell;

namespace
{
  void test_append_text_to_environment(environment& env_)
  {
    env_.append("#include <foo/bar.hpp>\n");
    
    JUST_ASSERT_EQUAL("#include <foo/bar.hpp>\n", env_.get_all());
  }
}

JUST_TEST_CASE(test_empty_in_memory_environment_is_empty)
{
  const config cfg = config::default_config();

  in_memory_environment env("foo", cfg);
  
  JUST_ASSERT_EQUAL("", env.get_all());
}

JUST_TEST_CASE(test_append_text_to_in_memory_environment)
{
  const config cfg = config::default_config();

  in_memory_environment env("foo", cfg);

  test_append_text_to_environment(env);
}

JUST_TEST_CASE(test_empty_header_file_environment_is_empty)
{
  config cfg = config::default_config();
  cfg.use_precompiled_headers = false;

  header_file_environment env(cfg);
  
  JUST_ASSERT_EQUAL("", env.get_all());
}

JUST_TEST_CASE(test_append_text_to_header_file_environment)
{
  config cfg = config::default_config();
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

