
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

#if defined __FreeBSD__ || defined __OpenBSD__ || defined __APPLE__
    free(line);
#else
    rl_free(line);
#endif
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

void readline_environment::set_rl_attempted_completion_function(
    readline_completion_function func)
{
  rl_attempted_completion_function = func;
}

}
