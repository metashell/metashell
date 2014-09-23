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

namespace
{
  template <int Len>
  metashell::parse_config_result parse_config(
    const char* (&args_)[Len],
    std::ostringstream* out_ = nullptr,
    std::ostringstream* err_ = nullptr
  )
  {
    return metashell::parse_config(Len, args_, out_, err_);
  }

  std::string first_line_of(const std::string& s_)
  {
    return s_.substr(0, s_.find('\n'));
  }

  std::string first_line_of(const std::ostringstream& s_)
  {
    return first_line_of(s_.str());
  }
}

JUST_TEST_CASE(test_recognising_extra_clang_arg)
{
  const char* args[] = {"metashell", "--", "foo"};

  const metashell::user_config cfg = parse_config(args).cfg;

  JUST_ASSERT_EQUAL(1u, cfg.extra_clang_args.size());
  JUST_ASSERT_EQUAL("foo", cfg.extra_clang_args.front());
}

JUST_TEST_CASE(test_extra_clang_args_are_not_parsed)
{
  const char* args[] = {"metashell", "--", "foo"};

  JUST_ASSERT(parse_config(args).should_run_shell());
}

JUST_TEST_CASE(test_default_template_depth)
{
  const char* args[] = {"metashell"};

  const metashell::user_config cfg = parse_config(args).cfg;

  JUST_ASSERT_EQUAL(256, cfg.max_template_depth);
}

JUST_TEST_CASE(test_template_depth_parsing)
{
  const char* args[] = {"metashell", "-ftemplate-depth=13"};

  const metashell::user_config cfg = parse_config(args).cfg;

  JUST_ASSERT_EQUAL(13, cfg.max_template_depth);
}

JUST_TEST_CASE(test_negative_template_depth_is_an_error)
{
  const char* args[] = {"metashell", "-ftemplate-depth=-1"};

  std::ostringstream err;
  const metashell::parse_config_result r = parse_config(args, nullptr, &err);

  JUST_ASSERT(!r.should_run_shell());
  JUST_ASSERT(r.should_error_at_exit());
  JUST_ASSERT_EQUAL("Template depth can not be negative.", first_line_of(err));
}

JUST_TEST_CASE(test_missing_template_depth_value_is_an_error)
{
  const char* args[] = {"metashell", "-ftemplate-depth="};

  std::ostringstream err;
  const metashell::parse_config_result r = parse_config(args, nullptr, &err);

  JUST_ASSERT(!r.should_run_shell());
  JUST_ASSERT(r.should_error_at_exit());
  JUST_ASSERT_EQUAL(
    "The value of -ftemplate-depth is missing.",
    first_line_of(err)
  );
}

JUST_TEST_CASE(test_non_numeric_template_depth_is_an_error)
{
  const char* args[] = {"metashell", "-ftemplate-depth=foo"};

  std::ostringstream err;
  const metashell::parse_config_result r = parse_config(args, nullptr, &err);

  JUST_ASSERT(!r.should_run_shell());
  JUST_ASSERT(r.should_error_at_exit());
  JUST_ASSERT_EQUAL("Invalid value for -ftemplate-depth.", first_line_of(err));
}

JUST_TEST_CASE(test_missing_equal_in_the_value_of_minus_f_is_an_error)
{
  const char* args[] = {"metashell", "-ftemplate-depth"};

  std::ostringstream err;
  const metashell::parse_config_result r = parse_config(args, nullptr, &err);

  JUST_ASSERT(!r.should_run_shell());
  JUST_ASSERT(r.should_error_at_exit());
  JUST_ASSERT_EQUAL(
    "-f has to be followed by template-depth=<value>",
    first_line_of(err)
  );
}

JUST_TEST_CASE(test_only_template_depth_can_follow_f)
{
  const char* args[] = {"metashell", "-ffoo="};

  std::ostringstream err;
  const metashell::parse_config_result r = parse_config(args, nullptr, &err);

  JUST_ASSERT(!r.should_run_shell());
  JUST_ASSERT(r.should_error_at_exit());
  JUST_ASSERT_EQUAL(
    "-f has to be followed by template-depth=<value>",
    first_line_of(err)
  );
}

