// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"
#include "interrupt_handler_override.hpp"
#include "console.hpp"

#include <metashell/shell.hpp>

#include <editline/readline.h>

#include <boost/bind.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include <cassert>

using namespace metashell;

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

  console::color color_of_token(boost::wave::token_id id_)
  {
    if (IS_CATEGORY(id_, boost::wave::CharacterLiteralTokenType))
    {
      return console::magenta;
    }
    else if (IS_CATEGORY(id_, boost::wave::FloatingLiteralTokenType))
    {
      return console::magenta;
    }
    else if (IS_CATEGORY(id_, boost::wave::IntegerLiteralTokenType))
    {
      return console::magenta;
    }
    else if (IS_CATEGORY(id_, boost::wave::StringLiteralTokenType))
    {
      return console::magenta;
    }
    else if (IS_CATEGORY(id_, boost::wave::BoolLiteralTokenType))
    {
      return console::magenta;
    }
    else if (IS_CATEGORY(id_, boost::wave::IdentifierTokenType))
    {
      return console::white;
    }
    else if (IS_CATEGORY(id_, boost::wave::KeywordTokenType))
    {
      return console::bright_green;
    }
    else if (IS_CATEGORY(id_, boost::wave::OperatorTokenType))
    {
      return console::white;
    }
    else if (IS_CATEGORY(id_, boost::wave::PPTokenType))
    {
      return console::magenta;
    }
    else
    {
      return console::white;
    }
  }

  void syntax_highlight(std::ostream& o_, const std::string& s_)
  {
    typedef boost::wave::cpplexer::lex_token<> token_type;

    for(
      boost::wave::cpplexer::lex_iterator<token_type>
        i(
          s_.begin(),
          s_.end(),
          token_type::position_type(shell::input_filename()),
          boost::wave::language_support(
            boost::wave::support_cpp
            | boost::wave::support_option_long_long
          )
        ),

        e;
      i != e;
      ++i
    )
    {
      console::text_color(color_of_token(*i));
      o_ << i->get_value();
    }
    console::reset();
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
  _syntax_highlight(config_.syntax_highlight)
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
    if (_syntax_highlight)
    {
      syntax_highlight(std::cout, s_);
    }
    else
    {
      std::cout << s_;
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


