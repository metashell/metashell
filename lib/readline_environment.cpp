
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/readline_environment.hpp>

#include <cstdlib>

#ifndef _WIN32
#  include <sys/ioctl.h>
#endif

#include <vector>

namespace metashell {

readline_environment::readline_environment() :
  last_rl_attempted_completion_function(rl_attempted_completion_function)
{

}

readline_environment::~readline_environment() {
  rl_attempted_completion_function = last_rl_attempted_completion_function;
}

boost::optional<std::string> readline_environment::readline(
    const std::string& prompt)
{
  char *line = ::readline(prompt.c_str());

  if (line) {
    std::string str(line);

    rl_free(line);
    return str;
  } else {
    return boost::none;
  }
}

namespace {

int line_length() {
#ifdef _WIN32
  return int(wcslen(_el_line_buffer));
#else
  return rl_end;
#endif
}

}

std::string readline_environment::get_edited_text() {
  return std::string(rl_line_buffer, rl_line_buffer + line_length());
}

void readline_environment::add_history(const std::string& line) {
  //TODO save/restore history
  std::vector<char> l(line.c_str(), line.c_str() + line.size() + 1);
  ::add_history(l.data());
}

void readline_environment::set_rl_attempted_completion_function(
    rl_completion_func_t func)
{
  rl_attempted_completion_function = func;
}


// This has nothing to do with readline, but it is better to have this
// in one common place
unsigned int readline_environment::width() const {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO info;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  return info.srWindow.Right - info.srWindow.Left + 1;
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
#endif
}

}
