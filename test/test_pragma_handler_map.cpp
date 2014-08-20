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

#include <metashell/pragma_handler_map.hpp>
#include <metashell/pragma_handler_interface.hpp>

#include <just/test.hpp>

using namespace metashell;

namespace
{
  class test_handler : public pragma_handler_interface
  {
  public:
    explicit test_handler(bool& run_flag_) : _run_flag(run_flag_) {}

    virtual pragma_handler_interface* clone() const
    {
      return new test_handler(_run_flag);
    }

    virtual std::string arguments() const { return "a|b|c"; }
    virtual std::string description() const { return "Foo bar"; }

    virtual void run(const command::iterator&, const command::iterator&) const
    {
      _run_flag = true;
    }
  private:
    bool& _run_flag;
  };
}

JUST_TEST_CASE(test_test_handler_sets_run_flag)
{
  bool flag = false;
  run(test_handler(flag), "foo");

  JUST_ASSERT(flag);
}

JUST_TEST_CASE(test_processing_non_existing_handler)
{
  pragma_handler_map m;
  const command cmd(/* #pragma metashell */ "foo");

  JUST_ASSERT_THROWS_SOMETHING(m.process(cmd.begin(), cmd.end()));
}

JUST_TEST_CASE(test_processing_existing_handler)
{
  bool foo_run = false;
  pragma_handler_map m;
  m.add("foo", test_handler(foo_run));
  const command cmd(/* #pragma metashell */ "foo");

  m.process(cmd.begin(), cmd.end());

  JUST_ASSERT(foo_run);
}

JUST_TEST_CASE(test_pragma_with_two_token_name_is_called)
{
  bool foo_bar_run = false;
  pragma_handler_map m;
  m.add("foo", "bar", test_handler(foo_bar_run));
  const command cmd(/* #pragma metashell */ "foo bar");

  m.process(cmd.begin(), cmd.end());

  JUST_ASSERT(foo_bar_run);
}

JUST_TEST_CASE(
  test_pragma_with_two_token_name_is_called_when_prefix_is_available
)
{
  bool foo_bar_run = false;
  bool foo_run = false;
  pragma_handler_map m;
  m.add("foo", test_handler(foo_run));
  m.add("foo", "bar", test_handler(foo_bar_run));
  const command cmd(/* #pragma metashell */ "foo bar");

  m.process(cmd.begin(), cmd.end());

  JUST_ASSERT(!foo_run);
  JUST_ASSERT(foo_bar_run);
}

JUST_TEST_CASE(test_pragma_prefix_is_selected_when_longer_version_is_available)
{
  bool foo_bar_run = false;
  bool foo_run = false;
  pragma_handler_map m;
  m.add("foo", test_handler(foo_run));
  m.add("foo", "bar", test_handler(foo_bar_run));
  const command cmd(/* #pragma metashell */ "foo x");

  m.process(cmd.begin(), cmd.end());

  JUST_ASSERT(foo_run);
  JUST_ASSERT(!foo_bar_run);
}

