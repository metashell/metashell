#ifndef METASHELL_TEST_SHELL_HPP
#define METASHELL_TEST_SHELL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/shell.hpp>

class test_shell : public metashell::shell
{
public:
  test_shell();

  virtual void display_normal(const std::string& s_) const;
  virtual void display_info(const std::string& s_) const;
  virtual void display_error(const std::string& s_) const;

  const std::string& output() const;
  const std::string& info() const;
  const std::string& error() const;
private:
  mutable std::string _output;
  mutable std::string _info;
  mutable std::string _error;
};

#endif

