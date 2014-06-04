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

#include <metashell/pragma_switch.hpp>
#include <metashell/metashell_pragma.hpp>
#include "test_shell.hpp"

#include <just/test.hpp>

using namespace metashell;

namespace
{
  void set_values(bool& set_to_true_, bool& set_to_value_, bool value_)
  {
    set_to_true_ = true;
    set_to_value_ = value_;
  }

  bool set_values(bool& set_to_true_)
  {
    set_to_true_ = true;
    return true;
  }

  template <bool B>
  bool always()
  {
    return B;
  }

  void ignore(bool) {}

  template <bool ExpectedResult>
  void test_callback_is_called(const std::string& arg_)
  {
    using boost::bind;
    using boost::ref;
  
    bool was_called = false;
    bool arg = !ExpectedResult;
    test_shell sh;
  
    pragma_switch
      p(
        "test",
        always<true>,
        bind(set_values, ref(was_called), ref(arg), _1),
        sh
      );
    p.run(*metashell_pragma::parse("#pragma metashell test " + arg_));
  
    JUST_ASSERT(was_called);
    JUST_ASSERT_EQUAL(ExpectedResult, arg);
  }
}

JUST_TEST_CASE(test_pragma_switch_calls_updating_callback)
{
  test_callback_is_called<true>("on");
  test_callback_is_called<true>("1");

  test_callback_is_called<false>("off");
  test_callback_is_called<false>("0");
}

JUST_TEST_CASE(test_pragma_switch_calls_query_callback)
{
  using boost::bind;
  using boost::ref;
  
  bool was_called = false;
  test_shell sh;
  
  pragma_switch p("test", bind(set_values, ref(was_called)), ignore, sh);
  p.run(*metashell_pragma::parse("#pragma metashell test"));
  
  JUST_ASSERT(was_called);
}


