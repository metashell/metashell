// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metashell.hpp"

#include <metashell/shell.hpp>
#include <metashell/version.hpp>
#include <metashell/token_iterator.hpp>

#include <boost/preprocessor/stringize.hpp>

using namespace metashell;

namespace
{
  bool is_environment_setup_command(const std::string& s_)
  {
    try
    {
      const token_iterator it = begin_tokens(s_), end;

      if (it == end)
      {
        // empty input is not a query
        return true;
      }
      else
      {
        const boost::wave::token_id id = *it;
        if (IS_CATEGORY(id, boost::wave::KeywordTokenType))
        {
          switch (id)
          {
          case boost::wave::T_BOOL:
          case boost::wave::T_CHAR:
          case boost::wave::T_CONST:
          case boost::wave::T_CONSTCAST:
          case boost::wave::T_DOUBLE:
          case boost::wave::T_DYNAMICCAST:
          case boost::wave::T_FLOAT:
          case boost::wave::T_INT:
          case boost::wave::T_LONG:
          case boost::wave::T_REINTERPRETCAST:
          case boost::wave::T_SHORT:
          case boost::wave::T_SIGNED:
          case boost::wave::T_SIZEOF:
          case boost::wave::T_STATICCAST:
          case boost::wave::T_UNSIGNED:
          case boost::wave::T_VOID:
          case boost::wave::T_VOLATILE:
          case boost::wave::T_WCHART:
            return false;
          default:
            return true;
          }
        }
        else
        {
          return IS_CATEGORY(id, boost::wave::PPTokenType);
        }
      }
    }
    catch (...)
    {
      return false;
    }
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

const char* shell::input_filename()
{
  return "<input>";
}

void shell::code_complete(
  const std::string& s_,
  std::set<std::string>& out_
) const
{
  metashell::code_complete(_buffer, s_, _config, out_);
}


