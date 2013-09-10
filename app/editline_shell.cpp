// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"
#include "syntax_highlighted_display.hpp"
#include "editline_tab_completion_override.hpp"
#include "interrupt_handler_override.hpp"
#include "console.hpp"

#include <metashell/shell.hpp>
#include <metashell/indenter.hpp>

#include <mindent/stream_display.hpp>

#include <editline/readline.h>

#include <boost/bind.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

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
    typedef boost::wave::cpplexer::lex_token<> token_type;
    typedef boost::wave::cpplexer::lex_iterator<token_type> iterator_type;
 
    std::for_each(
      iterator_type(
        s_.begin(),
        s_.end(),
        token_type::position_type(shell::input_filename()),
        boost::wave::language_support(
          boost::wave::support_cpp
          | boost::wave::support_option_long_long
        )
      ),
      iterator_type(),
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

  editline_tab_completion_override ovr1(tab_completion);
  interrupt_handler_override ovr2(bind(&editline_shell::cancel_operation,this));

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


