// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"
#include "syntax_highlighted_display.hpp"
#include "override_guard.hpp"
#include "interrupt_handler_override.hpp"
#include "console.hpp"

#include <metashell/shell.hpp>
#include <metashell/indenter.hpp>
#include <metashell/token_iterator.hpp>

#include <mindent/stream_display.hpp>

#include <editline/readline.h>

#include <boost/bind.hpp>

#include <sys/ioctl.h>

#include <algorithm>
#include <string>
#include <iostream>

#include <cassert>

using namespace metashell;

namespace
{
  template <console::color Color>
  void display(const std::string& s_)
  {
    if (s_ != "")
    {
      console::text_color(Color);
      std::cout << s_;
      console::reset();
      std::cout << std::endl;
    }
  }

  void syntax_highlight(std::ostream& o_, const std::string& s_)
  {
    std::for_each(
      begin_tokens(s_),
      token_iterator(),
      syntax_highlighted_display()
    );

    console::reset();
  }

  int console_width()
  {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
  }
}

editline_shell* editline_shell::_instance = 0;

editline_shell::~editline_shell()
{
  assert(_instance);
  _instance = 0;
}

editline_shell::editline_shell(const metashell::config& config_) :
  shell(config_),
  _syntax_highlight(config_.syntax_highlight),
  _indent(config_.indent)
{
  assert(!_instance);
  _instance = this;
}

void editline_shell::run()
{
  using boost::bind;

  char empty_string[] = "";
  override_guard<char*> ovr1(rl_basic_word_break_characters, empty_string);

  override_guard<char** (*)(const char*, int, int)>
    ovr2(rl_attempted_completion_function, tab_completion);

  interrupt_handler_override ovr3(bind(&editline_shell::cancel_operation,this));

  for (;;)
  {
    if (char* l = readline(prompt().c_str()))
    {
      add_history(l);
      line_available(l);
      free(l);
    }
    else
    {
      break;
    }
  }
}

char* editline_shell::tab_generator(const char* text_, int state_)
{
  assert(_instance);

  static std::set<std::string> values;
  static std::set<std::string>::const_iterator pos;

  if (!state_) // init
  {
    _instance->code_complete(text_, values);
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
    std::copy(str.begin(), str.end(), s);
    s[str.length()] = 0;
    ++pos;
    return s;
  }
  return 0;
}

char** editline_shell::tab_completion(const char* text_, int start_, int end_)
{
  return rl_completion_matches(const_cast<char*>(text_), &tab_generator);
}

void editline_shell::display_normal(const std::string& s_) const
{
  if (s_ != "")
  {
    if (_indent)
    {
      if (_syntax_highlight)
      {
        indent(console_width(), 2, syntax_highlighted_display(), s_);
        console::reset();
      }
      else
      {
        indent(console_width(), 2, mindent::stream_display(std::cout), s_);
      }
    }
    else
    {
      if (_syntax_highlight)
      {
        syntax_highlight(std::cout, s_);
      }
      else
      {
        std::cout << s_;
      }
    }
    std::cout << std::endl;
  }
}

void editline_shell::display_info(const std::string& s_) const
{
  display<console::bright_cyan>(s_);
}

void editline_shell::display_error(const std::string& s_) const
{
  display<console::bright_red>(s_);
}


