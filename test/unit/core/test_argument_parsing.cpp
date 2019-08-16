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

#include <metashell/core/parse_config.hpp>

#include <metashell/mock/environment_detector.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <vector>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;

namespace
{
  core::parse_config_result
  parse_config(std::initializer_list<const char*> args_,
               std::ostringstream* out_ = nullptr,
               std::ostringstream* err_ = nullptr)
  {
    std::vector<const char*> args{"metashell"};
    args.insert(args.end(), args_.begin(), args_.end());
    NiceMock<mock::environment_detector> env_detector;

    ON_CALL(env_detector, on_windows()).WillByDefault(Return(false));
    ON_CALL(env_detector, on_osx()).WillByDefault(Return(false));

    return core::parse_config(
        args.size(), args.data(),
        std::map<data::real_engine_name, core::engine_entry>(), env_detector,
        out_, err_);
  }

  bool fails_and_displays_error(std::initializer_list<const char*> args_)
  {
    std::ostringstream err;
    const core::parse_config_result r = parse_config(args_, nullptr, &err);

    if (const core::exit* e = mpark::get_if<core::exit>(&r))
    {
      return e->with_error && !err.str().empty();
    }
    else
    {
      return false;
    }
  }

  data::config parse_valid_config(std::initializer_list<const char*> args_)
  {
    return mpark::get<data::config>(parse_config(args_));
  }
}

TEST(argument_parsing, recognising_engine_args)
{
  const data::command_line_argument_list engine_args(
      parse_valid_config({"--", "foo"}).active_shell_config().engine_args);

  ASSERT_EQ(1u, engine_args.size());
  ASSERT_EQ(data::command_line_argument("foo"), engine_args.front());
}

TEST(argument_parsing, engine_args_are_not_parsed)
{
  auto r = parse_config({"--", "foo"});
  ASSERT_TRUE(mpark::get_if<data::config>(&r));
}

TEST(argument_parsing, saving_is_enabled_by_default_during_parsing)
{
  const data::config cfg = parse_valid_config({});

  ASSERT_TRUE(cfg.saving_enabled);
}

TEST(argument_parsing, disabling_saving)
{
  const data::config cfg = parse_valid_config({"--disable_saving"});

  ASSERT_FALSE(cfg.saving_enabled);
}

TEST(argument_parsing, default_console_type_is_readline)
{
  const data::config cfg = parse_valid_config({});

  ASSERT_EQ(data::console_type::readline, cfg.con_type);
}

TEST(argument_parsing, setting_console_type_to_plain)
{
  const data::config cfg = parse_valid_config({"--console", "plain"});

  ASSERT_EQ(data::console_type::plain, cfg.con_type);
}

TEST(argument_parsing, setting_console_type_to_readline)
{
  const data::config cfg = parse_valid_config({"--console", "readline"});

  ASSERT_EQ(data::console_type::readline, cfg.con_type);
}

TEST(argument_parsing, setting_console_type_to_json)
{
  const data::config cfg = parse_valid_config({"--console", "json"});

  ASSERT_EQ(data::console_type::json, cfg.con_type);
}

TEST(argument_parsing, splash_is_enabled_by_default)
{
  const data::config cfg = parse_valid_config({});

  ASSERT_TRUE(cfg.splash_enabled);
}

TEST(argument_parsing, disabling_splash)
{
  const data::config cfg = parse_valid_config({"--nosplash"});

  ASSERT_FALSE(cfg.splash_enabled);
}

TEST(argument_parsing, logging_mode_is_none_by_default)
{
  const data::config cfg = parse_valid_config({});

  ASSERT_EQ(data::logging_mode::none, cfg.log_mode);
}

TEST(argument_parsing, logging_to_console)
{
  const data::config cfg = parse_valid_config({"--log", "-"});

  ASSERT_EQ(data::logging_mode::console, cfg.log_mode);
}

TEST(argument_parsing, logging_to_file)
{
  const data::config cfg = parse_valid_config({"--log", "/tmp/foo.txt"});

  ASSERT_EQ(data::logging_mode::file, cfg.log_mode);
  ASSERT_EQ("/tmp/foo.txt", cfg.log_file);
}

TEST(argument_parsing, it_is_an_error_to_specify_log_twice)
{
  const core::parse_config_result r =
      parse_config({"--log", "-", "--log", "-"});

  const core::exit* const e = mpark::get_if<core::exit>(&r);

  ASSERT_TRUE(e);
  ASSERT_TRUE(e->with_error);
}

TEST(argument_parsing, decommissioned_arguments_provide_an_error_message)
{
  ASSERT_TRUE(fails_and_displays_error({"-I/usr/include"}));
  ASSERT_TRUE(fails_and_displays_error({"--include", "/usr/include"}));
  ASSERT_TRUE(fails_and_displays_error({"-DFOO=bar"}));
  ASSERT_TRUE(fails_and_displays_error({"--define", "FOO=bar"}));
  ASSERT_TRUE(fails_and_displays_error({"--std", "c++11"}));
  ASSERT_TRUE(fails_and_displays_error({"-w"}));
  ASSERT_TRUE(fails_and_displays_error({"--no_warnings"}));
  ASSERT_TRUE(fails_and_displays_error({"-ftemplate-depth=13"}));
  ASSERT_TRUE(fails_and_displays_error({"-stdlib=libstdc++"}));
}

TEST(argument_parsing, not_specifying_the_engine)
{
  ASSERT_EQ(data::engine_name(data::auto_engine_name()),
            parse_valid_config({}).active_shell_config().engine);
}

TEST(argument_parsing, specifying_the_engine)
{
  ASSERT_EQ(
      data::engine_name(data::real_engine_name::null),
      parse_valid_config({"--engine", "null"}).active_shell_config().engine);
}

TEST(argument_parsing, preprocessor_mode_is_off_by_default)
{
  ASSERT_FALSE(parse_valid_config({}).active_shell_config().preprocessor_mode);
}

TEST(argument_parsing, preprocessor_mode_is_set_from_command_line)
{
  ASSERT_TRUE(parse_valid_config({"--preprocessor"})
                  .active_shell_config()
                  .preprocessor_mode);
}
