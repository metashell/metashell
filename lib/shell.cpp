// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metashell.hpp"

#include <metashell/shell.hpp>
#include <metashell/version.hpp>

#include <boost/xpressive/xpressive.hpp>

#include <boost/preprocessor/stringize.hpp>

using namespace metashell;

namespace
{
  bool is_environment_setup_command(const std::string& s_)
  {
    using boost::xpressive::sregex;
    using boost::xpressive::blank;
    using boost::xpressive::bos;
    using boost::xpressive::smatch;
    using boost::xpressive::as_xpr;
    
    const sregex re =
      bos >> *blank >> (
        as_xpr('#') // preprocessor directive
        |
        (
          (as_xpr("using") | as_xpr("template") | as_xpr("typedef")) >> blank
        ) // keywords
      );

    smatch what;
    return regex_search(s_, what, re);
  }

  void display(const result& r_, shell& s_)
  {
    if (!r_.info.empty())
    {
      s_.display_info(r_.info);
    }

    for (
      std::vector<std::string>::const_iterator
        i = r_.errors.begin(),
        e = r_.errors.end();
      i != e;
      ++i
    )
    {
      s_.display_error(*i);
    }
    if (!r_.has_errors())
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
  if (_config.verbose)
  {
    display_info("Verbose mode: ON\n");
  }
}

void shell::line_available(const std::string& s_)
{
  if (is_environment_setup_command(s_))
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

bool shell::store_in_buffer(const std::string& s_)
{
  const std::string new_buffer = append_to_buffer(_buffer, s_);
  const result r = validate_code(new_buffer, _config);
  const bool success = !r.has_errors();
  if (success)
  {
    _buffer = new_buffer;
  }
  display(r, *this);
  return success;
}


