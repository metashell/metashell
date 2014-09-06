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
#include "override_guard.hpp"
#include "interrupt_handler_override.hpp"

#include <metashell/shell.hpp>
#include <metashell/indenter.hpp>
#include <metashell/command.hpp>

#include <just/console.hpp>

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
  boost::optional<just::console::color> color_of_token(const token& t_)
  {
    using just::console::color;
    using boost::optional;
  
    switch (t_.category())
    {
    case token_category::character_literal:
    case token_category::floating_literal:
    case token_category::integer_literal:
    case token_category::string_literal:
    case token_category::bool_literal:
    case token_category::preprocessor:
      return color::magenta;
    case token_category::keyword:
      return color::bright_green;
    case token_category::comment:
      return color::green;
    default:
      return optional<color>();
    }
  }

  void display(just::console::color c_, const std::string& s_)
  {
    if (s_ != "")
    {
      just::console::text_color(c_);
      std::cout << s_;
      just::console::reset();
      std::cout << std::endl;
    }
  }

  void display_syntax_highlighted(const token& t_)
  {
    if (const boost::optional<just::console::color> c = color_of_token(t_))
    {
      just::console::text_color(*c);
    }
    else
    {
      just::console::reset();
    }
    std::cout << t_.value();
  }

  void syntax_highlight(const std::string& s_)
  {
    const command cmd(s_);
    std::for_each(cmd.begin(), cmd.end(), display_syntax_highlighted);
    just::console::reset();
  }

  int line_length()
  {
#ifdef _WIN32
    return int(wcslen(_el_line_buffer));
#else
    return rl_end;
#endif
  }

  std::string get_edited_text()
  {
    return std::string(rl_line_buffer, rl_line_buffer + line_length());
  }

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

readline_shell::readline_shell(const metashell::config& config_) :
  shell(config_),
  _syntax_highlight(config_.syntax_highlight),
  _indent(config_.indent)
{
  assert(!_instance);
  _instance = this;
}

void readline_shell::add_history(const std::string& s_)
{
  std::vector<char> s(s_.begin(), s_.end());
  s.push_back(0);
  ::add_history(&s[0]);
}

void readline_shell::run()
{
  override_guard<char** (*)(const char*, int, int)>
    ovr2(rl_attempted_completion_function, tab_completion);

  interrupt_handler_override ovr3([this]() { this->cancel_operation(); });

  for (char* l = 0; !stopped() && (l = readline(prompt().c_str()));)
  {
    const std::string line(l);
    rl_free(l);
    line_available(line);
  }
}

char* readline_shell::tab_generator(const char* text_, int state_)
{
  assert(_instance);

  static std::set<std::string> values;
  static std::set<std::string>::const_iterator pos;

  if (!state_) // init
  {
    const std::string edited_text = get_edited_text();
    _instance->code_complete(
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

void readline_shell::display_normal(const std::string& s_) const
{
  if (s_ != "")
  {
    if (_indent)
    {
      if (_syntax_highlight)
      {
        indent(width(), 2, display_syntax_highlighted, s_, input_filename());
        just::console::reset();
      }
      else
      {
        indent(
          width(),
          2,
          mindent::stream_display(std::cout),
          s_,
          input_filename()
        );
      }
    }
    else
    {
      if (_syntax_highlight)
      {
        syntax_highlight(s_);
      }
      else
      {
        std::cout << s_;
      }
    }
    std::cout << std::endl;
  }
}

void readline_shell::display_info(const std::string& s_) const
{
  std::cout << s_;
}

void readline_shell::display_error(const std::string& s_) const
{
  display(just::console::color::bright_red, s_);
}

unsigned int readline_shell::width() const
{
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

