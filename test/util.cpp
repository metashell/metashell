// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "util.hpp"
#include "test_shell.hpp"

#include <boost/test/unit_test.hpp>

std::string get_output(const std::string& input_, const std::string& test_code_)
{
  test_shell sh;
  if (!test_code_.empty())
  {
    const bool r = sh.store_in_buffer(test_code_);
    BOOST_CHECK(r);
  }
  sh.line_available(input_);
  BOOST_CHECK_EQUAL("", sh.info());
  BOOST_CHECK_EQUAL("", sh.error());
  return sh.output();
}

