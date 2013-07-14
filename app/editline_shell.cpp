// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"
#include "interrupt_handler_override.hpp"
#include "console.hpp"

#include <editline/readline.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <vector>

#include <cassert>

namespace
{
  class editline_tab_completion_override : boost::noncopyable
  {
  private:
    typedef char** (*callback)(const char*, int, int);
  public:
    editline_tab_completion_override(callback cb_) :
      _old(rl_attempted_completion_function)
    {
      rl_attempted_completion_function = cb_;
    }

    ~editline_tab_completion_override()
    {
      rl_attempted_completion_function = _old;
    }
  private:
    char** (*_old)(const char*, int, int);
  };

  bool starts_with(const std::string& prefix_, const std::string& s_)
  {
    if (prefix_.length() <= s_.length())
    {
      for (int i = 0, e = prefix_.length(); i != e; ++i)
      {
        if (prefix_[i] != s_[i])
        {
          return false;
        }
      }
      return true;
    }
    else
    {
      return false;
    }
  }

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
}

editline_shell* editline_shell::_instance = 0;

editline_shell::~editline_shell()
{
  assert(_instance);
  _instance = 0;
}

editline_shell::editline_shell(const metashell::config& config_) :
  shell(config_)
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
    std::cout << s_ << std::endl;
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


