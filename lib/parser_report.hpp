#ifndef METASHELL_PARSER_REPORT_HPP
#define METASHELL_PARSER_REPORT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>

namespace metashell
{
  struct parser_report
  {
    std::vector<std::string> errors;
  };
}

#endif

