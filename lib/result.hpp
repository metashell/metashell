#ifndef METASHELL_RESULT_HPP
#define METASHELL_RESULT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>

namespace metashell
{
  struct result
  {
    std::string output;
    std::vector<std::string> errors;
    std::string info;

    result();

    template <class InputIt>
    result(
      const std::string& output_,
      InputIt begin_errors_,
      InputIt end_errors_,
      const std::string& info_
    ) :
      output(output_),
      errors(begin_errors_, end_errors_),
      info(info_)
    {}

    bool has_errors() const;
  };
}

#endif

