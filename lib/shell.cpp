// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/metashell.hpp>
#include "indenter.hpp"
#include "exception.hpp"

#include <metashell/shell.hpp>
#include <metashell/version.hpp>
#include <metashell/token_iterator.hpp>
#include <metashell/in_memory_environment.hpp>
#include <metashell/header_file_environment.hpp>
#include <metashell/metashell_pragma.hpp>

#include <boost/wave/cpplexer/cpplexer_exceptions.hpp>

#include <boost/foreach.hpp>

#include <cctype>

using namespace metashell;

namespace
{
  void display(const result& r_, shell& s_)
  {
    if (!r_.info.empty())
    {
      s_.display_info(r_.info);
    }

    BOOST_FOREACH(const std::string& i, r_.errors)
    {
      s_.display_error(i);
    }
    if (!r_.has_errors())
    {
      s_.display_normal(r_.output);
    }
  }

  bool has_non_whitespace(const std::string& s_)
  {
    BOOST_FOREACH(char c, s_)
    {
      if (!std::isspace(c))
      {
        return true;
      }
    }
    return false;
  }

  bool is_empty_line(const std::string& s_, const std::string& input_filename_)
  {
    try
    {
      token_iterator i = begin_tokens(s_, input_filename_), e;
      while (i != e)
      {
        try
        {
          if (
            !(
              IS_CATEGORY(*i, boost::wave::WhiteSpaceTokenType)
              || IS_CATEGORY(*i, boost::wave::EOFTokenType)
              || IS_CATEGORY(*i, boost::wave::EOLTokenType)
            )
          )
          {
            return false;
          }
          ++i;
        }
        catch (const boost::wave::cpplexer::lexing_exception& e)
        {
          if (!e.is_recoverable())
          {
            throw;
          }
        }
      }
    }
    catch (...)
    {
      return false;
    }
    return true;
  }
}

shell::shell(const config& config_) :
  _env(),
  _config(config_),
  _stopped(false)
{
  rebuild_environment();
  init();
}

shell::shell(const config& config_, environment* env_) :
  _env(env_),
  _config(config_),
  _stopped(false)
{
  init();
}

shell::~shell() {}

void shell::cancel_operation() {}

void shell::display_splash() const
{
  const std::string version_desc =
    #include "version_desc.hpp"
  ;

  indenter ind(width(), " * ");
  ind
    .raw("/*")
    .left_align("Template metaprogramming shell " + version());

  if (!version_desc.empty())
  {
    ind.left_align(version_desc);
  }

  display_normal(
    ind
      .empty_line()
      .left_align(
        "Metashell Copyright (C) 2013 Abel Sinkovics (abel@sinkovics.hu)"
      )
      .left_align(
        "This program comes with ABSOLUTELY NO WARRANTY. This is free software,"
        " and you are welcome to redistribute it under certain conditions;"
        " for details visit <http://www.gnu.org/licenses/>."
      )
      .empty_line()
      .left_align("Based on")
      .left_align(
        metashell::libclang_version(),
        " *              ",
        " *   libclang   "
      )
      .left_align(
        metashell::wave_version(),
        " *              ",
        " *   Boost.Wave "
      )
      .left_align(
        metashell::readline_version(),
        " *              ",
        " *   Readline   "
      )
      .empty_line()
      .left_align(
        _config.use_precompiled_headers ?
          "Using precompiled headers" :
          "Not using precompiled headers"
      )
      .empty_line()
      .left_align("Getting help: #pragma metashell help")
      .raw(" */")
      .str()
  );
  if (_config.verbose)
  {
    display_normal(
      indenter(width(), "// ")
        .left_align("Verbose mode: ON")
        .str()
    );
  }
}

void shell::line_available(const std::string& s_)
{
  try
  {
    if (has_non_whitespace(s_))
    {
      if (_prev_line != s_)
      {
        add_history(s_);
        _prev_line = s_;
      }

      if (!is_empty_line(s_, input_filename()))
      {
        if (boost::optional<token_iterator> p = parse_pragma(s_))
        {
          _pragma_handlers.process(*p);
        }
        else if (is_environment_setup_command(s_, input_filename()))
        {
          store_in_buffer(s_);
        }
        else
        {
          display(eval_tmp(*_env, s_, _config, input_filename()), *this);
        }
      }
    }
  }
  catch (const std::exception& e)
  {
    display_error(std::string("Error: ") + e.what());
  }
  catch (...)
  {
    display_error("Unknown error");
  }
}

std::string shell::prompt() const
{
  return "> ";
}

bool shell::store_in_buffer(const std::string& s_)
{
  const result r = validate_code(s_, _config, *_env, input_filename());
  const bool success = !r.has_errors();
  if (success)
  {
    try
    {
      _env->append(s_);
    }
    catch (const std::exception& e)
    {
      display_error(e.what());
      return false;
    }
  }
  display(r, *this);
  return success;
}

const char* shell::input_filename()
{
  return "<stdin>";
}

void shell::code_complete(
  const std::string& s_,
  std::set<std::string>& out_
) const
{
  metashell::code_complete(*_env, s_, _config, input_filename(), out_);
}

void shell::init()
{
  _env->append(
    "#define __METASHELL\n"
    "#define __METASHELL_MAJOR " BOOST_PP_STRINGIZE(METASHELL_MAJOR) "\n"
    "#define __METASHELL_MINOR " BOOST_PP_STRINGIZE(METASHELL_MINOR) "\n"
    "#define __METASHELL_PATCH " BOOST_PP_STRINGIZE(METASHELL_PATCH) "\n"

    "namespace metashell { "
      "namespace impl { "
        "template <class T> "
        "struct wrap {}; "

        "template <class T> "
        "typename T::tag tag_of(::metashell::impl::wrap<T>); "
        
        "void tag_of(...); "
      "} "
      
      "template <class Tag> "
      "struct format_impl "
      "{ "
        "typedef format_impl type; "
        
        "template <class T> "
        "struct apply { typedef T type; }; "
      "}; "

      "template <class T> "
      "struct format : "
        "::metashell::format_impl<"
          "decltype(::metashell::impl::tag_of(::metashell::impl::wrap<T>()))"
        ">::template apply<T>"
        "{}; "

      ""
    "}"
    "\n"
  );

  // TODO: move it to initialisation later
  _pragma_handlers = pragma_handler_map::build_default(*this);
}

const pragma_handler_map& shell::pragma_handlers() const
{
  return _pragma_handlers;
}

void shell::verbose(bool enabled_)
{
  _config.verbose = enabled_;
}

bool shell::verbose() const
{
  return _config.verbose;
}

bool shell::stopped() const
{
  return _stopped;
}

void shell::stop()
{
  _stopped = true;
}

bool shell::using_precompiled_headers() const
{
  return _config.use_precompiled_headers;
}

void shell::using_precompiled_headers(bool enabled_)
{
  _config.use_precompiled_headers = enabled_;
  rebuild_environment();
}

const environment& shell::env() const
{
  return *_env;
}

void shell::rebuild_environment(const std::string& content_)
{
  if (_config.use_precompiled_headers)
  {
    _env.reset(new header_file_environment(_config));
  }
  else
  {
    _env.reset(new in_memory_environment("__metashell_internal", _config));
  }
  if (!content_.empty())
  {
    _env->append(content_);
  }
}

void shell::rebuild_environment()
{
  rebuild_environment(_env ? _env->get_all() : std::string());
}

void shell::push_environment()
{
  _environment_stack.push(_env->get_all());
}

void shell::pop_environment()
{
  if (_environment_stack.empty())
  {
    throw exception("The environment stack is empty.");
  }
  else
  {
    rebuild_environment(_environment_stack.top());
    _environment_stack.pop();
  }
}

void shell::display_environment_stack_size()
{
  if (_environment_stack.empty())
  {
    display_normal("// Environment stack is empty\n");
  }
  else if (_environment_stack.size() == 1)
  {
    display_normal("// Environment stack has 1 entry\n");
  }
  else
  {
    std::ostringstream s;
    s
      << "// Environment stack has " << _environment_stack.size()
      << " entries\n";
    display_normal(s.str());
  }
}

