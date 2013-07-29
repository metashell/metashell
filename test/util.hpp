#ifndef METASHELL_TEST_UTIL_HPP
#define METASHELL_TEST_UTIL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>

std::string get_output(
  const std::string& input_,
  const std::string& test_code_ = std::string()
);

#endif

