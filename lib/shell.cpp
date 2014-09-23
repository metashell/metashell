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
#include <metashell/wave_tokeniser.hpp>
#include "indenter.hpp"
#include "exception.hpp"

#include <metashell/shell.hpp>
#include <metashell/version.hpp>
#include <metashell/in_memory_environment.hpp>
#include <metashell/header_file_environment.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/command.hpp>
#include <metashell/to_string.hpp>

#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace metashell;

namespace
{
  std::string max_template_depth_info(int depth_)
  {
    std::ostringstream s;
    s << "Maximum template depth: " << depth_;
    return s.str();
  }

  void display(const result& r_, shell& s_)
  {
    if (!r_.info.empty())
    {
      s_.display_info(r_.info);
    }

    for (const std::string& i : r_.errors)
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
    for (char c : s_)
    {
      if (!std::isspace(c))
      {
        return true;
      }
    }
    return false;
  }

  bool is_empty_line(const command& cmd_)
  {
    return
      std::find_if(
        cmd_.begin(),
        cmd_.end(),
        [] (const token& t_) -> bool
        {
          const token_category c = t_.category();
          return
            c != token_category::whitespace
            && c != token_category::comment;
        }
      ) == cmd_.end();
  }

  const char default_env[] =
    "#define __METASHELL\n"
    "#define __METASHELL_MAJOR " TO_STRING(METASHELL_MAJOR) "\n"
    "#define __METASHELL_MINOR " TO_STRING(METASHELL_MINOR) "\n"
    "#define __METASHELL_PATCH " TO_STRING(METASHELL_PATCH) "\n"

    "namespace metashell { "
      "namespace impl { "
        "template <class T> "
        "struct wrap {}; "

        "template <class T> "
        "typename T::tag* tag_of(::metashell::impl::wrap<T>); "

        "void* tag_of(...); "

        "template <class T> "
        "struct remove_ptr; "

        "template <class T> "
        "struct remove_ptr<T*> { typedef T type; }; "
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
          "typename ::metashell::impl::remove_ptr<"
            "decltype(::metashell::impl::tag_of(::metashell::impl::wrap<T>()))"
          ">::type"
        ">::template apply<T>"
        "{}; "

      ""
    "}"
    "\n";
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
        "                             Andras Kucsma  (andras.kucsma@gmail.com)"
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
#ifdef USE_EDITLINE
        " *   Libedit    "
#else
        " *   Readline   "
#endif
      )
      .empty_line()
      .left_align(
        _config.use_precompiled_headers ?
          "Using precompiled headers" :
          "Not using precompiled headers"
      )
      .left_align(max_template_depth_info(_config.max_template_depth))
      .empty_line()
      .left_align("Getting help: #msh help")
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
    if (s_.empty() || s_.back() != '\\')
    {
      const std::string s = _line_prefix + s_;
      _line_prefix.clear();

      const command cmd(s);

      if (has_non_whitespace(s))
      {
        if (_prev_line != s)
        {
          add_history(s);
          _prev_line = s;
        }

        if (!is_empty_line(cmd))
        {
          if (boost::optional<command::iterator> p = parse_pragma(cmd))
          {
            _pragma_handlers.process(*p, cmd.end());
          }
          else if (is_environment_setup_command(cmd))
          {
            store_in_buffer(s);
          }
          else
          {
            run_metaprogram(s);
          }
        }
      }
    }
    else
    {
      _line_prefix += s_.substr(0, s_.size() - 1);
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
  return _line_prefix.empty() ? "> " : "...> ";
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
  ::display(r, *this);
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
  metashell::code_complete(*_env, s_, input_filename(), out_);
}

void shell::init()
{
  _env->append(default_env);

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

void shell::run_metaprogram(const std::string& s_)
{
  display(eval_tmp_formatted(*_env, s_, _config, input_filename()), *this);
}

void shell::reset_environment()
{
  rebuild_environment("");
  _env->append(default_env);
}

const config& shell::get_config() const {
  return _config;
}

