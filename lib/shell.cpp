// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metashell.hpp"

#include <metashell/shell.hpp>
#include <metashell/version.hpp>

#include <boost/preprocessor/stringize.hpp>

using namespace metashell;

namespace
{
  bool is_preprocessor_directive(const std::string& s_)
  {
    for (std::string::const_iterator i = s_.begin(), e = s_.end(); i != e; ++i)
    {
      switch (*i)
      {
      case ' ':
      case '\t':
        break;
      case '#':
        return true;
      default: 
        return false;
      }
    }
    return false;
  }

  void display(const result& r_, shell& s_)
  {
    for (
      std::vector<std::string>::const_iterator
        i = r_.report.errors.begin(),
        e = r_.report.errors.end();
      i != e;
      ++i
    )
    {
      s_.display_error(*i);
    }
    if (r_.report.errors.empty())
    {
      s_.display_normal(r_.output);
    }
  }
}

shell::shell(const config& config_) : _config(config_) {}

shell::~shell() {}

void shell::cancel_operation() {}

void shell::display_splash() const
{
  display_normal(
    "/*\n"
    " *  Template metaprogramming shell "
      BOOST_PP_STRINGIZE(METASHELL_MAJOR)
      "." BOOST_PP_STRINGIZE(METASHELL_MINOR) "\n"
    " */\n"
  );
}

void shell::line_available(const std::string& s_)
{
  if (is_preprocessor_directive(s_))
  {
    store_in_buffer(s_);
  }
  else
  {
    display(eval_tmp(_buffer, s_, _config), *this);
  }
}

std::string shell::prompt() const
{
  return "> ";
}

void shell::store_in_buffer(const std::string& s_)
{
  const std::string new_buffer = append_to_buffer(_buffer, s_);
  const result r = validate_code(new_buffer, _config);
  if (!r.has_errors())
  {
    _buffer = new_buffer;
  }
  display(r, *this);
}


