// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "result.hpp"

using namespace metashell;

result::result() {}

bool result::has_errors() const
{
  return !errors.empty();
}

