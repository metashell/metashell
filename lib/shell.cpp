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

#include <metashell/shell.hpp>
#include <metashell/version.hpp>
#include <metashell/header_file_environment.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/command.hpp>
#include <metashell/to_string.hpp>
#include <metashell/exception.hpp>
#include <metashell/null_history.hpp>

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

  void display(const result& r_, iface::displayer& displayer_)
  {
    if (!r_.info.empty())
    {
      displayer_.show_raw_text(r_.info);
    }

    for (const std::string& i : r_.errors)
    {
      displayer_.show_error(i);
    }
    if (!r_.has_errors() && !r_.output.empty())
    {
      displayer_.show_type(data::type(r_.output));
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

shell::shell(const config& config_, logger* logger_) :
  _env(),
  _config(config_),
  _stopped(false),
  _logger(logger_)
{
  rebuild_environment();
  init(nullptr);
}

shell::shell(
  const config& config_,
  command_processor_queue& cpq_,
  logger* logger_
) :
  _env(),
  _config(config_),
  _stopped(false),
  _logger(logger_)
{
  rebuild_environment();
  init(&cpq_);
}

shell::shell(
  const config& config_,
  std::unique_ptr<environment> env_,
  command_processor_queue& cpq_,
  logger* logger_
) :
  _env(std::move(env_)),
  _config(config_),
  _stopped(false),
  _logger(logger_)
{
  init(&cpq_);
}

void shell::cancel_operation() {}

void shell::display_splash(iface::displayer& displayer_)
{
  const std::string version_desc =
    #include "version_desc.hpp"
  ;

  const paragraph empty_line("");

  text splash_text;
  splash_text.paragraphs.push_back(
    paragraph("Template metaprogramming shell " + version())
  );

  if (!version_desc.empty())
  {
    splash_text.paragraphs.push_back(paragraph(version_desc));
  }

  splash_text.paragraphs.push_back(empty_line);
  splash_text.paragraphs.push_back(
    paragraph("Metashell Copyright (C) 2013 Abel Sinkovics (abel@sinkovics.hu)")
  );
  splash_text.paragraphs.push_back(
    paragraph(
      "                             Andras Kucsma  (andras.kucsma@gmail.com)"
    )
  );
  splash_text.paragraphs.push_back(
    paragraph(
      "This program comes with ABSOLUTELY NO WARRANTY. This is free software,"
      " and you are welcome to redistribute it under certain conditions; for"
      " details visit <http://www.gnu.org/licenses/>."
    )
  );
  splash_text.paragraphs.push_back(empty_line);
  splash_text.paragraphs.push_back(paragraph("Based on"));
  splash_text.paragraphs.push_back(
    paragraph(metashell::libclang_version(), "             ", "  libclang   ")
  );
  splash_text.paragraphs.push_back(
    paragraph(metashell::wave_version(), "             ", "  Boost.Wave ")
  );
  splash_text.paragraphs.push_back(
    paragraph(
      metashell::readline_version(),
      "             ",
#ifdef USE_EDITLINE
      "  Libedit    "
#else
      "  Readline   "
#endif
    )
  );
  splash_text.paragraphs.push_back(empty_line);
  splash_text.paragraphs.push_back(
    paragraph(
      _config.use_precompiled_headers ?
        "Using precompiled headers" :
        "Not using precompiled headers"
    )
  );
  splash_text.paragraphs.push_back(
    paragraph(max_template_depth_info(_config.max_template_depth))
  );
  splash_text.paragraphs.push_back(empty_line);
  splash_text.paragraphs.push_back(paragraph("Getting help: #msh help"));

  displayer_.show_comment(splash_text);

  if (_config.verbose)
  {
    displayer_.show_comment(text("Verbose mode: ON"));
  }
}

void shell::line_available(
  const std::string& s_,
  iface::displayer& displayer_,
  iface::history& history_
)
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
          history_.add(s);
          _prev_line = s;
        }

        if (!is_empty_line(cmd))
        {
          if (boost::optional<command::iterator> p = parse_pragma(cmd))
          {
            _pragma_handlers.process(*p, cmd.end(), displayer_);
          }
          else if (is_environment_setup_command(cmd))
          {
            store_in_buffer(s, displayer_);
          }
          else
          {
            run_metaprogram(s, displayer_);
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
    displayer_.show_error(std::string("Error: ") + e.what());
  }
  catch (...)
  {
    displayer_.show_error("Unknown error");
  }
}

std::string shell::prompt() const
{
  return _line_prefix.empty() ? ">" : "...>";
}

bool shell::store_in_buffer(const std::string& s_, iface::displayer& displayer_)
{
  const result r = validate_code(s_, _config, *_env, input_filename(), _logger);
  const bool success = !r.has_errors();
  if (success)
  {
    try
    {
      _env->append(s_);
    }
    catch (const std::exception& e)
    {
      displayer_.show_error(e.what());
      return false;
    }
  }
  ::display(r, displayer_);
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
  try
  {
    metashell::code_complete(*_env, s_, input_filename(), out_, _logger);
  }
  catch (...)
  {
    // ignore
  }
}

void shell::init(command_processor_queue* cpq_)
{
  _env->append(default_env);

  // TODO: move it to initialisation later
  _pragma_handlers = pragma_handler_map::build_default(*this, cpq_, _logger);
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

environment& shell::env()
{
  return *_env;
}

const environment& shell::env() const
{
  return *_env;
}

void shell::rebuild_environment(const std::string& content_)
{
  _env.reset(new header_file_environment(_config, _logger));
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

void shell::display_environment_stack_size(iface::displayer& displayer_)
{
  if (_environment_stack.empty())
  {
    displayer_.show_comment(text("Environment stack is empty"));
  }
  else if (_environment_stack.size() == 1)
  {
    displayer_.show_comment(text("Environment stack has 1 entry"));
  }
  else
  {
    std::ostringstream s;
    s << "Environment stack has " << _environment_stack.size() << " entries";
    displayer_.show_comment(text(s.str()));
  }
}

void shell::run_metaprogram(const std::string& s_, iface::displayer& displayer_)
{
  display(
    eval_tmp_formatted(*_env, s_, _config, input_filename(), _logger),
    displayer_
  );
}

void shell::reset_environment()
{
  rebuild_environment("");
  _env->append(default_env);
}

const config& shell::get_config() const {
  return _config;
}

void shell::line_available(const std::string& s_, iface::displayer& displayer_)
{
  null_history h;
  line_available(s_, displayer_, h);
}

