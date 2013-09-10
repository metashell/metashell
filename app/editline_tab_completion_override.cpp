// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_tab_completion_override.hpp"

#include <editline/readline.h>

editline_tab_completion_override::editline_tab_completion_override(
  callback cb_
) :
  _old(rl_attempted_completion_function)
{
  rl_attempted_completion_function = cb_;
}

editline_tab_completion_override::~editline_tab_completion_override()
{
  rl_attempted_completion_function = _old;
}


