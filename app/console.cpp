// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "console.hpp"

#ifdef _MSC_VER
  #include <windows.h>
#else
  #include <iostream>
#endif

using namespace console;

#ifdef _MSC_VER

namespace
{
#ifdef R
  #error R is already defined
#endif
#define R FOREGROUND_RED

#ifdef G
  #error G is already defined
#endif
#define G FOREGROUND_GREEN

#ifdef B
  #error B is already defined
#endif
#define B FOREGROUND_BLUE

#ifdef BRIGHT
  #error BRIGHT is already defined
#endif
#define BRIGHT FOREGROUND_INTENSITY

  WORD code_of_color(color c_)
  {
    switch (c_)
    {
    case black:          return 0;
    case red:            return          R;
    case green:          return              G;
    case yellow:         return          R | G;
    case blue:           return                  B;
    case magenta:        return          R     | B;
    case cyan:           return              G | B;
    case gray:           return          R | G | B;
    case bright_red:     return BRIGHT | R;
    case bright_green:   return BRIGHT |     G;
    case bright_yellow:  return BRIGHT | R | G;
    case bright_blue:    return BRIGHT |         B;
    case bright_magenta: return BRIGHT | R     | B;
    case bright_cyan:    return BRIGHT |     G | B;
    case white:          return BRIGHT | R | G | B;
    }
  }

#undef R
#undef G
#undef B
#undef BRIGHT

  void set_console_attribute(WORD attr_)
  {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr_);
  }
}

void console::reset()
{
  set_console_attribute(code_of_color(gray));
}

void console::text_color(color c_)
{
  set_console_attribute(code_of_color(c_));
}

#else

namespace
{
#ifdef CONSOLE_COLOR
  #error CONSOLE_COLOR already defined
#endif
#define CONSOLE_COLOR(c) "\033[" #c "m"

  const char* code_of_color(color c_)
  {
    switch (c_)
    {
    case black:          return CONSOLE_COLOR(30);
    case red:            return CONSOLE_COLOR(31);
    case green:          return CONSOLE_COLOR(32);
    case yellow:         return CONSOLE_COLOR(33);
    case blue:           return CONSOLE_COLOR(34);
    case magenta:        return CONSOLE_COLOR(35);
    case cyan:           return CONSOLE_COLOR(36);
    case gray:           return CONSOLE_COLOR(37);
    case bright_red:     return CONSOLE_COLOR(1;31);
    case bright_green:   return CONSOLE_COLOR(1;32);
    case bright_yellow:  return CONSOLE_COLOR(1;33);
    case bright_blue:    return CONSOLE_COLOR(1;34);
    case bright_magenta: return CONSOLE_COLOR(1;35);
    case bright_cyan:    return CONSOLE_COLOR(1;36);
    case white:          return CONSOLE_COLOR(1;37);
    }
  }

#undef CONSOLE_COLOR
}

void console::reset()
{
  std::cout << "\033[0m";
}

void console::text_color(color c_)
{
  std::cout << code_of_color(c_);
}

#endif


