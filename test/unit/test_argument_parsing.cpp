// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/parse_config.hpp>

#include <just/test.hpp>

#include <sstream>
#include <iostream>
#include <vector>

using namespace metashell;

namespace
{
  parse_config_result parse_config(
    std::initializer_list<const char*> args_,
    std::ostringstream* out_ = nullptr,
    std::ostringstream* err_ = nullptr
  )
  {
    std::vector<const char*> args{"metashell"};
    args.insert(args.end(), args_.begin(), args_.end());

    return metashell::parse_config(args.size(), args.data(), out_, err_);
  }

  bool fails_and_displays_error(std::initializer_list<const char*> args_)
  {
    std::ostringstream err;
    const parse_config_result r = parse_config(args_, nullptr, &err);
  
    return
      parse_config_result::action_t::exit_with_error == r.action
      && !err.str().empty();
  }
}

JUST_TEST_CASE(test_recognising_extra_clang_arg)
{
  const data::user_config cfg = parse_config({"--", "foo"}).cfg;

  JUST_ASSERT_EQUAL(1u, cfg.extra_clang_args.size());
  JUST_ASSERT_EQUAL("foo", cfg.extra_clang_args.front());
}

JUST_TEST_CASE(test_extra_clang_args_are_not_parsed)
{
  JUST_ASSERT(parse_config({"--", "foo"}).should_run_shell());
}

JUST_TEST_CASE(test_saving_is_disabled_by_default_during_parsing)
{
  const data::user_config cfg = parse_config({}).cfg;

  JUST_ASSERT(!cfg.saving_enabled);
}

JUST_TEST_CASE(test_enabling_saving)
{
  const data::user_config cfg = parse_config({"--enable_saving"}).cfg;

  JUST_ASSERT(cfg.saving_enabled);
}

JUST_TEST_CASE(test_default_console_type_is_readline)
{
  const data::user_config cfg = parse_config({}).cfg;

  JUST_ASSERT_EQUAL(data::console_type::readline, cfg.con_type);
}

JUST_TEST_CASE(test_setting_console_type_to_plain)
{
  const data::user_config cfg = parse_config({"--console", "plain"}).cfg;

  JUST_ASSERT_EQUAL(data::console_type::plain, cfg.con_type);
}

JUST_TEST_CASE(test_setting_console_type_to_readline)
{
  const data::user_config cfg = parse_config({"--console", "readline"}).cfg;

  JUST_ASSERT_EQUAL(data::console_type::readline, cfg.con_type);
}

JUST_TEST_CASE(test_setting_console_type_to_json)
{
  const data::user_config cfg = parse_config({"--console", "json"}).cfg;

  JUST_ASSERT_EQUAL(data::console_type::json, cfg.con_type);
}

JUST_TEST_CASE(test_splash_is_enabled_by_default)
{
  const data::user_config cfg = parse_config({}).cfg;

  JUST_ASSERT(cfg.splash_enabled);
}

JUST_TEST_CASE(test_disabling_splash)
{
  const data::user_config cfg = parse_config({"--nosplash"}).cfg;

  JUST_ASSERT(!cfg.splash_enabled);
}

JUST_TEST_CASE(test_logging_mode_is_none_by_default)
{
  const data::user_config cfg = parse_config({}).cfg;

  JUST_ASSERT_EQUAL(data::logging_mode::none, cfg.log_mode);
}

JUST_TEST_CASE(test_logging_to_console)
{
  const data::user_config cfg = parse_config({"--log", "-"}).cfg;

  JUST_ASSERT_EQUAL(data::logging_mode::console, cfg.log_mode);
}

JUST_TEST_CASE(test_logging_to_file)
{
  const data::user_config cfg = parse_config({"--log", "/tmp/foo.txt"}).cfg;

  JUST_ASSERT_EQUAL(data::logging_mode::file, cfg.log_mode);
  JUST_ASSERT_EQUAL("/tmp/foo.txt", cfg.log_file);
}

JUST_TEST_CASE(test_it_is_an_error_to_specify_log_twice)
{
  const parse_config_result r = parse_config({"--log", "-", "--log", "-"});

  JUST_ASSERT(!r.should_run_shell());
  JUST_ASSERT(r.should_error_at_exit());
}

JUST_TEST_CASE(test_decommissioned_arguments_provide_an_error_message)
{
  JUST_ASSERT(fails_and_displays_error({"-I/usr/include"}));
  JUST_ASSERT(fails_and_displays_error({"--include", "/usr/include"}));
  JUST_ASSERT(fails_and_displays_error({"-DFOO=bar"}));
  JUST_ASSERT(fails_and_displays_error({"--define", "FOO=bar"}));
  JUST_ASSERT(fails_and_displays_error({"--std", "c++11"}));
  JUST_ASSERT(fails_and_displays_error({"-w"}));
  JUST_ASSERT(fails_and_displays_error({"--no_warnings"}));
  JUST_ASSERT(fails_and_displays_error({"-ftemplate-depth=13"}));
  JUST_ASSERT(fails_and_displays_error({"-stdlib=libstdc++"}));
}

JUST_TEST_CASE(test_not_specifying_the_engine)
{
  const data::user_config cfg = parse_config({}).cfg;

  JUST_ASSERT_EQUAL("internal", cfg.engine);
}

JUST_TEST_CASE(test_specifying_the_engine)
{
  const data::user_config cfg = parse_config({"--engine", "foo"}).cfg;

  JUST_ASSERT_EQUAL("foo", cfg.engine);
}

