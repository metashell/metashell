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

#include <boost/test/unit_test.hpp>

namespace
{
  template <int Len>
  metashell::parse_config_result parse_config(
    metashell::config& cfg_,
    const char* (&args_)[Len]
  )
  {
    return metashell::parse_config(cfg_, Len, args_);
  }
}

BOOST_AUTO_TEST_CASE(test_recognising_extra_clang_arg)
{
  const char* args[] = {"metashell", "--", "foo"};

  metashell::config cfg = metashell::config::empty;
  parse_config(cfg, args);

  BOOST_REQUIRE_EQUAL(1, cfg.extra_clang_args.size());
  BOOST_CHECK_EQUAL("foo", cfg.extra_clang_args.front());
}

BOOST_AUTO_TEST_CASE(test_extra_clang_args_are_not_parsed)
{
  const char* args[] = {"metashell", "--", "foo"};

  metashell::config cfg = metashell::config::empty;
  BOOST_CHECK_EQUAL(metashell::run_shell, parse_config(cfg, args));
}

