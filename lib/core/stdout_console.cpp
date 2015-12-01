// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/stdout_console.hpp>

#ifdef _WIN32
#  include <windows.h>
#else
#  include <sys/ioctl.h>
#endif

#include <iostream>

using namespace metashell;

void stdout_console::show(const data::colored_string& s_)
{
  print_to_cout(s_);
}

void stdout_console::new_line()
{
  std::cout << std::endl;
}

iface::console::user_answer stdout_console::ask_for_continuation()
{
  std::string line;
  while (true) {
    std::cout << "Next page (RETURN), Show all (a), Quit (q): ";

    if (!std::getline(std::cin, line)) {
      new_line();
      return iface::console::user_answer::quit;
    }
    if (line.empty()) {
      return iface::console::user_answer::next_page;
    }
    if (line == "a" || line == "A") {
      return iface::console::user_answer::show_all;
    }
    if (line == "q" || line == "Q") {
      return iface::console::user_answer::quit;
    }
  }
}

int stdout_console::width() const
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.srWindow.Right - info.srWindow.Left;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

int stdout_console::height() const
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.srWindow.Bottom - info.srWindow.Top + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
#endif
}
