#ifndef EDITLINE_SHELL_HPP
#define EDITLINE_SHELL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/shell.hpp>
#include <metashell/config.hpp>

#include <vector>
#include <string>

class editline_shell : public metashell::shell
{
public:
  editline_shell(const metashell::config& config_);
  virtual ~editline_shell();

  virtual void display_normal(const std::string& s_) const;
  virtual void display_info(const std::string& s_) const;
  virtual void display_error(const std::string& s_) const;

  void run();
private:
  bool _syntax_highlight;
  bool _indent;

  static char* tab_generator(const char* text_, int state_);
  static char** tab_completion(const char* text_, int start_, int end_);

  static editline_shell* _instance;
};

#endif

