// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    return 0; // keep GCC happy
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
    return ""; // keep GCC happy
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


