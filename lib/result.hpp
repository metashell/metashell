#ifndef METASHELL_RESULT_HPP
#define METASHELL_RESULT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parser_report.hpp"

#include <string>

namespace metashell
{
  struct result
  {
    std::string output;
    parser_report report;

    bool has_errors() const
    {
      return !report.errors.empty();
    }
  };
}

#endif

