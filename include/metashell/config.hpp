#ifndef METASHELL_CONFIG_HPP
#define METASHELL_CONFIG_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>

namespace metashell
{
  struct config
  {
    config();

    std::vector<std::string> include_path;
    bool verbose;
    bool syntax_highlight;
  };
}

#endif

