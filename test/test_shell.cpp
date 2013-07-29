// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "test_shell.hpp"

test_shell::test_shell() : metashell::shell(metashell::config()) {}

void test_shell::display_normal(const std::string& s_) const
{
  _output += s_;
}

void test_shell::display_info(const std::string& s_) const
{
  _info += s_;
}

void test_shell::display_error(const std::string& s_) const
{
  _error += s_;
}

const std::string& test_shell::output() const
{
  return _output;
}

const std::string& test_shell::info() const
{
  return _info;
}

const std::string& test_shell::error() const
{
  return _error;
}

