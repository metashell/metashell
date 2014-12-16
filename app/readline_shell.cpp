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

#include "readline_shell.hpp"
#include "interrupt_handler_override.hpp"

#include <metashell/shell.hpp>
#include <metashell/indenter.hpp>
#include <metashell/command.hpp>
#include <metashell/highlight_syntax.hpp>

#include <mindent/stream_display.hpp>

#ifdef USE_EDITLINE
#  include <editline/readline.h>
#else
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

#ifndef _WIN32
#  include <sys/ioctl.h>
#endif

#include <boost/optional.hpp>

#include <algorithm>
#include <string>
#include <iostream>
#include <iterator>

#include <cassert>

using namespace metashell;

namespace
{
#ifdef _WIN32
  template <class T>
  stdext::checked_array_iterator<T*> array_begin(T* array_, int len_)
  {
    return stdext::checked_array_iterator<T*>(array_, len_);
  }
#else
  template <class T>
  T* array_begin(T* array_, int)
  {
    return array_;
  }
#endif
}

readline_shell* readline_shell::_instance = 0;
int readline_shell::_completion_end = 0;

readline_shell::~readline_shell()
{
  assert(_instance);
  _instance = 0;
}

readline_shell::readline_shell(
  const metashell::config& config_,
  iface::displayer& displayer_
) :
  _shell(config_, displayer_)
{
  assert(!_instance);

  _shell.history(_history);
  _instance = this;

  _shell.display_splash();
}

void readline_shell::run()
{
  _readline_environment.set_rl_attempted_completion_function(
      tab_completion);

  interrupt_handler_override
    ovr3([this]() { this->_shell.cancel_operation(); });

  for (
    boost::optional<std::string> line;
    !_shell.stopped()
    && (line = _readline_environment.readline(_shell.prompt()));
  )
  {
    _shell.line_available(*line);
  }
  std::cout << std::endl;
}

char* readline_shell::tab_generator(const char* text_, int state_)
{
  assert(_instance);

  static std::set<std::string> values;
  static std::set<std::string>::const_iterator pos;

  if (!state_) // init
  {
    const std::string edited_text =
      _instance->_readline_environment.get_edited_text();

    _instance->_shell.code_complete(
      std::string(edited_text.begin(), edited_text.begin() + _completion_end),
      values
    );
    pos = values.begin();
  }

  if (pos == values.end())
  {
    return 0;
  }
  else
  {
    const std::string str = text_ + *pos;
    char* s = new char[str.length() + 1];
    std::copy(str.begin(), str.end(), array_begin(s, str.length() + 1));
    s[str.length()] = 0;
    ++pos;
    return s;
  }
  return 0;
}

char** readline_shell::tab_completion(const char* text_, int, int end_)
{
  _completion_end = end_;
  return rl_completion_matches(const_cast<char*>(text_), &tab_generator);
}

