#ifndef CONSOLE_HPP
#define CONSOLE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace console
{
  enum color {
    black,
    red,
    bright_red,
    green,
    bright_green,
    yellow,
    bright_yellow,
    blue,
    bright_blue,
    magenta,
    bright_magenta,
    cyan,
    bright_cyan,
    gray,
    white
  };

  void reset();
  void text_color(color c_);
}

#endif


