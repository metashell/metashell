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

#include <metashell/main_shell/metashell_pragma.hpp>
#include <metashell/main_shell/process_pragma.hpp>
#include <metashell/main_shell/shell.hpp>

#include <metashell/core/code_complete.hpp>
#include <metashell/core/command.hpp>
#include <metashell/core/header_file_environment.hpp>
#include <metashell/core/is_environment_setup_command.hpp>
#include <metashell/core/null_history.hpp>
#include <metashell/core/version.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/feature_not_supported.hpp>
#include <metashell/data/to_string.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>

namespace metashell
{
  namespace main_shell
  {
    namespace
    {
      bool determine_echo(const data::shell_config& cfg_)
      {
        return cfg_.preprocessor_mode;
      }

      bool determine_show_cpp_errors(const data::shell_config& cfg_)
      {
        return !cfg_.preprocessor_mode;
      }

      bool determine_evaluate_metaprograms(const data::shell_config& cfg_)
      {
        return !cfg_.preprocessor_mode;
      }

      void make_failure(data::result& r_, const std::string& msg_)
      {
        r_.successful = false;
        r_.output.clear();
        r_.error = msg_;
      }

      std::string extend(const std::string& s_, int width_)
      {
        const int len = s_.length();
        return len < width_ ? s_ + std::string(width_ - len, ' ') : s_;
      }

      void display(const data::result& r_,
                   iface::displayer& displayer_,
                   bool show_as_type_)
      {
        if (!r_.info.empty())
        {
          displayer_.show_raw_text(r_.info);
        }
        if (!r_.successful)
        {
          displayer_.show_error(r_.error);
        }
        if (r_.successful && !r_.output.empty())
        {
          if (show_as_type_)
          {
            displayer_.show_type(data::type(r_.output));
          }
          else
          {
            displayer_.show_cpp_code(data::cpp_code(r_.output));
          }
        }
      }

      bool is_empty_line(const data::command& cmd_)
      {
        return std::find_if(
                   cmd_.begin(), cmd_.end(), [](const data::token& t_) -> bool {
                     const data::token_category c = category(t_);
                     return c != data::token_category::whitespace &&
                            c != data::token_category::comment;
                   }) == cmd_.end();
      }

      data::result eval_tmp_formatted(const iface::environment& env_,
                                      const data::cpp_code& tmp_exp_,
                                      bool use_precompiled_headers_,
                                      iface::type_shell& type_shell_,
                                      core::logger* logger_)
      {
        using std::string;
        using std::pair;

        METASHELL_LOG(logger_,
                      "Checking if metaprogram can be evaluated without "
                      "metashell::format: " +
                          tmp_exp_.value());

        const data::result simple =
            type_shell_.eval(env_, tmp_exp_, use_precompiled_headers_);

        METASHELL_LOG(
            logger_,
            !simple.successful ?
                "Errors occured during metaprogram evaluation. Displaying "
                "errors"
                " coming from the metaprogram without metashell::format" :
                "No errors occured during metaprogram evaluation. "
                "Re-evaluating it"
                " with metashell::format");

        return simple.successful ?
                   type_shell_.eval(env_,
                                    data::cpp_code("::metashell::format<") +
                                        tmp_exp_ + data::cpp_code(">::type"),
                                    use_precompiled_headers_) :
                   simple;
      }

      const char default_env[] =
"#define __METASHELL\n"
"#define __METASHELL_MAJOR " TO_STRING(METASHELL_MAJOR) "\n"
"#define __METASHELL_MINOR " TO_STRING(METASHELL_MINOR) "\n"
"#define __METASHELL_PATCH " TO_STRING(METASHELL_PATCH) "\n"

R"(
#if defined __cplusplus && __cplusplus > 200000

namespace metashell {
  namespace impl {
    template <class T>
    struct wrap {}; // This is a metashell internal type

    template <class T>
    typename T::tag* tag_of(::metashell::impl::wrap<T>);

    void* tag_of(...);

    template <class T>
    struct remove_ptr;

    template <class T>
    struct remove_ptr<T*> { typedef T type; };
  } // namespace impl

  template <class Tag>
  struct format_impl
  {
    typedef format_impl type;

    template <class T>
    struct apply { typedef T type; };
  };

  template <class T>
  struct format :
    ::metashell::format_impl<
      typename ::metashell::impl::remove_ptr<
        decltype(::metashell::impl::tag_of(::metashell::impl::wrap<T>()))
      >::type
    >::template apply<T>
  {};
} // namespace metashell

#endif
)";

      iface::type_shell* try_to_get_shell(iface::engine& engine_)
      {
        try
        {
          return &engine_.type_shell();
        }
        catch (const data::feature_not_supported<iface::type_shell>&)
        {
          return nullptr;
        }
      }

      bool code_complete_pp_directive(const data::cpp_code& prefix_,
                                      bool metashell_extensions_,
                                      data::command::const_iterator begin_,
                                      data::command::const_iterator end_,
                                      iface::main_shell& shell_,
                                      data::code_completion& out_)
      {
        if (begin_ == end_)
        {
          bool found = false;
          for (const auto& directive :
               {data::cpp_code{"define"}, data::cpp_code{"if"},
                data::cpp_code{"ifdef"}, data::cpp_code{"ifndef"},
                data::cpp_code{"include"}, data::cpp_code{"else"},
                data::cpp_code{"elif"}, data::cpp_code{"endif"},
                data::cpp_code{"error"}, data::cpp_code{"line"},
                data::cpp_code{"pragma"}, data::cpp_code{"undef"},
                data::cpp_code{"warning"}})
          {
            if (starts_with(directive, prefix_))
            {
              out_.insert(data::user_input{directive.substr(prefix_.size())});
              found = true;
            }
          }

          if (metashell_extensions_ &&
              starts_with(data::cpp_code{"msh"}, prefix_))
          {
            out_.insert(data::user_input{"msh"}.substr(prefix_.size()));
            found = true;
          }
          return found;
        }

        if (metashell_extensions_ && prefix_ == data::cpp_code{"pragma"})
        {
          begin_ = data::skip_all_whitespace(begin_, end_);
          if (begin_ == end_)
          {
            out_.insert(data::user_input{"metashell"});
            return false;
          }
          else if (type_of(*begin_) == data::token_type::identifier)
          {
            const data::cpp_code val = value(*begin_);
            const data::cpp_code metashell_{"metashell"};

            if (val == metashell_)
            {
              ++begin_;
              auto[completions, pragmas] =
                  core::code_complete::pragma_metashell(
                      data::skip_all_whitespace(begin_, end_), end_,
                      shell_.pragma_handlers());
              out_.insert(std::move(completions));
              for (const auto& p : pragmas)
              {
                out_.insert(p.first->code_complete(p.second, end_, shell_));
              }
              return true;
            }
            else if (starts_with(metashell_, val))
            {
              out_.insert(data::user_input{metashell_.substr(val.size())});
              return true;
            }
          }
          return false;
        }

        return false;
      }

      template <data::token_category... Cs>
      bool any_of(data::token_category category_)
      {
        const data::token_category cats[] = {Cs...};
        return std::find(std::begin(cats), std::end(cats), category_) !=
               std::end(cats);
      }
    }

    shell::shell(
        const data::config& config_,
        const boost::filesystem::path& internal_dir_,
        const boost::filesystem::path& env_filename_,
        std::function<std::unique_ptr<iface::engine>(const data::shell_config&)>
            engine_builder_,
        std::vector<data::real_engine_name> available_engines_,
        std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>>
            pragma_handlers_,
        core::logger* logger_,
        std::unique_ptr<iface::environment> env_)
      : _internal_dir(internal_dir_),
        _env_filename(env_filename_),
        _env(std::move(env_)),
        _config(config_),
        _pragma_handlers(std::move(pragma_handlers_)),
        _stopped(false),
        _logger(logger_),
        _engine_builder(std::move(engine_builder_)),
        _available_engines{std::move(available_engines_)},
        _echo(determine_echo(config_.active_shell_config())),
        _show_cpp_errors(
            determine_show_cpp_errors(config_.active_shell_config())),
        _evaluate_metaprograms(
            determine_evaluate_metaprograms(config_.active_shell_config()))
    {
      if (!_env)
      {
        rebuild_environment();
      }
      _env->append(data::cpp_code(default_env));
    }

    void shell::cancel_operation() {}

    void shell::display_splash(
        iface::displayer& displayer_,
        const std::map<std::string, std::string>& dependency_versions_)
    {
      using data::paragraph;

      const std::string version_desc =
#include "version_desc.hpp"
          ;

      const paragraph empty_line("");

      data::text splash_text;
      splash_text.paragraphs.push_back(
          paragraph("Template metaprogramming shell " + core::version()));

      if (!version_desc.empty())
      {
        splash_text.paragraphs.push_back(paragraph(version_desc));
      }

      splash_text.paragraphs.push_back(empty_line);
      splash_text.paragraphs.push_back(paragraph(
          "Metashell Copyright (C) 2015 Abel Sinkovics (abel@sinkovics.hu)"));
      splash_text.paragraphs.push_back(
          paragraph("                             Andras Kucsma  "
                    "(andras.kucsma@gmail.com)"));
      splash_text.paragraphs.push_back(
          paragraph("This program comes with ABSOLUTELY NO WARRANTY. This is "
                    "free software,"
                    " and you are welcome to redistribute it under certain "
                    "conditions; for"
                    " details visit <http://www.gnu.org/licenses/>."));
      splash_text.paragraphs.push_back(empty_line);
      splash_text.paragraphs.push_back(paragraph("Based on"));
      for (const auto& d : dependency_versions_)
      {
        splash_text.paragraphs.push_back(
            paragraph(d.second, "             ", extend("  " + d.first, 13)));
      }
      splash_text.paragraphs.push_back(empty_line);
      splash_text.paragraphs.push_back(
          paragraph(_config.active_shell_config().use_precompiled_headers ?
                        "Using precompiled headers" :
                        "Not using precompiled headers"));
      splash_text.paragraphs.push_back(empty_line);
      splash_text.paragraphs.push_back(paragraph("Getting help: #msh help"));

      displayer_.show_comment(splash_text);

      if (_config.verbose)
      {
        displayer_.show_comment(data::text("Verbose mode: ON"));
      }
    }

    void shell::line_available(const data::user_input& s_,
                               iface::displayer& displayer_,
                               iface::history& history_)
    {
      try
      {
        if (!ends_with(s_, data::user_input("\\")))
        {
          const data::user_input s = _line_prefix + s_;
          _line_prefix.clear();

          const data::command cmd = core::to_command(data::cpp_code(s));

          if (has_non_whitespace(s))
          {
            if (_prev_line != s)
            {
              history_.add(s);
              _prev_line = s;
            }

            if (!is_empty_line(cmd))
            {
              if (std::optional<data::command::iterator> p = parse_pragma(cmd))
              {
                process_pragma(
                    _pragma_handlers, *p, cmd.end(), *this, displayer_);
              }
              else if (!_echo ||
                       preprocess(displayer_, data::cpp_code(s), true))
              {
                if (core::is_environment_setup_command(cmd))
                {
                  store_in_buffer(data::cpp_code(s), displayer_);
                }
                else
                {
                  run_metaprogram(data::cpp_code(s), displayer_);
                }
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

    bool shell::store_in_buffer(const data::cpp_code& s_,
                                iface::displayer& displayer_)
    {
      const data::result r = engine().cpp_validator().validate_code(
          s_ + data::cpp_code("\n"), _config, *_env,
          enabled(data::shell_flag::use_precompiled_headers));

      if (r.successful)
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
      if (_show_cpp_errors || r.successful)
      {
        display(r, displayer_, true);
      }
      return r.successful;
    }

    data::code_completion shell::code_complete(const data::user_input& s_,
                                               bool metashell_extensions_)
    {
      data::code_completion result;

      try
      {
        const data::command cmd = core::to_command(data::cpp_code{s_});
        const auto e = cmd.end();
        auto i = data::skip_all_whitespace(cmd.begin(), e);
        if (i != e)
        {
          switch (type_of(*i))
          {
          case data::token_type::p_define:
          case data::token_type::p_if:
          case data::token_type::p_ifdef:
          case data::token_type::p_ifndef:
          case data::token_type::p_else:
          case data::token_type::p_elif:
          case data::token_type::p_endif:
          case data::token_type::p_error:
          case data::token_type::p_line:
          case data::token_type::p_pragma:
          case data::token_type::p_undef:
          case data::token_type::p_warning:
          case data::token_type::p_include:
          {
            data::cpp_code directive{format_token(*i).substr(1)};

            for (++i;
                 i != e && any_of<data::token_category::identifier,
                                  data::token_category::keyword>(category(*i));
                 ++i)
            {
              directive += data::cpp_code{format_token(*i)};
            }

            if (code_complete_pp_directive(
                    directive, metashell_extensions_, i, e, *this, result))
            {
              return result;
            }
          }
          break;
          case data::token_type::operator_pound:
          {
            ++i;
            i = data::skip_all_whitespace(i, e);

            const auto directive = i == e ? data::cpp_code{} : value(*i);
            if (i != e)
            {
              ++i;
            }

            if (code_complete_pp_directive(
                    directive, metashell_extensions_, i, e, *this, result))
            {
              return result;
            }
            else if (metashell_extensions_ &&
                     directive == data::cpp_code{"msh"})
            {
              auto[completions, pragmas] =
                  core::code_complete::pragma_metashell(
                      i, e, pragma_handlers());
              result.insert(std::move(completions));
              for (const auto& p : pragmas)
              {
                result.insert(p.first->code_complete(p.second, e, *this));
              }
              return result;
            }
          }
          break;
          case data::token_type::unknown:
          case data::token_type::identifier:
          case data::token_type::character_literal:
          case data::token_type::floating_literal:
          case data::token_type::integer_literal:
          case data::token_type::string_literal:
          case data::token_type::bool_literal:
          case data::token_type::c_comment:
          case data::token_type::cpp_comment:
          case data::token_type::whitespace:
          case data::token_type::continue_line:
          case data::token_type::new_line:
          case data::token_type::keyword_asm:
          case data::token_type::keyword_auto:
          case data::token_type::keyword_bool:
          case data::token_type::keyword_break:
          case data::token_type::keyword_case:
          case data::token_type::keyword_catch:
          case data::token_type::keyword_char:
          case data::token_type::keyword_class:
          case data::token_type::keyword_const:
          case data::token_type::keyword_constexpr:
          case data::token_type::keyword_const_cast:
          case data::token_type::keyword_continue:
          case data::token_type::keyword_default:
          case data::token_type::keyword_delete:
          case data::token_type::keyword_do:
          case data::token_type::keyword_double:
          case data::token_type::keyword_dynamic_cast:
          case data::token_type::keyword_else:
          case data::token_type::keyword_enum:
          case data::token_type::keyword_explicit:
          case data::token_type::keyword_export:
          case data::token_type::keyword_extern:
          case data::token_type::keyword_float:
          case data::token_type::keyword_for:
          case data::token_type::keyword_friend:
          case data::token_type::keyword_goto:
          case data::token_type::keyword_if:
          case data::token_type::keyword_inline:
          case data::token_type::keyword_int:
          case data::token_type::keyword_long:
          case data::token_type::keyword_mutable:
          case data::token_type::keyword_namespace:
          case data::token_type::keyword_new:
          case data::token_type::keyword_operator:
          case data::token_type::keyword_private:
          case data::token_type::keyword_protected:
          case data::token_type::keyword_public:
          case data::token_type::keyword_register:
          case data::token_type::keyword_reinterpret_cast:
          case data::token_type::keyword_return:
          case data::token_type::keyword_short:
          case data::token_type::keyword_signed:
          case data::token_type::keyword_sizeof:
          case data::token_type::keyword_static:
          case data::token_type::keyword_static_cast:
          case data::token_type::keyword_struct:
          case data::token_type::keyword_switch:
          case data::token_type::keyword_template:
          case data::token_type::keyword_this:
          case data::token_type::keyword_throw:
          case data::token_type::keyword_try:
          case data::token_type::keyword_typedef:
          case data::token_type::keyword_typeid:
          case data::token_type::keyword_typename:
          case data::token_type::keyword_union:
          case data::token_type::keyword_unsigned:
          case data::token_type::keyword_using:
          case data::token_type::keyword_virtual:
          case data::token_type::keyword_void:
          case data::token_type::keyword_volatile:
          case data::token_type::keyword_wchar_t:
          case data::token_type::keyword_while:
          case data::token_type::operator_bitwise_and:
          case data::token_type::operator_logical_and:
          case data::token_type::operator_assign:
          case data::token_type::operator_bitwise_and_assign:
          case data::token_type::operator_bitwise_or:
          case data::token_type::operator_bitwise_or_assign:
          case data::token_type::operator_bitwise_xor:
          case data::token_type::operator_bitwise_xor_assign:
          case data::token_type::operator_comma:
          case data::token_type::operator_colon:
          case data::token_type::operator_divide:
          case data::token_type::operator_divide_assign:
          case data::token_type::operator_dot:
          case data::token_type::operator_dotstar:
          case data::token_type::operator_ellipsis:
          case data::token_type::operator_equal:
          case data::token_type::operator_greater:
          case data::token_type::operator_greater_equal:
          case data::token_type::operator_left_brace:
          case data::token_type::operator_less:
          case data::token_type::operator_less_equal:
          case data::token_type::operator_left_paren:
          case data::token_type::operator_left_bracket:
          case data::token_type::operator_minus:
          case data::token_type::operator_minus_assign:
          case data::token_type::operator_minus_minus:
          case data::token_type::operator_modulo:
          case data::token_type::operator_modulo_assign:
          case data::token_type::operator_logical_not:
          case data::token_type::operator_not_equal:
          case data::token_type::operator_logical_or:
          case data::token_type::operator_plus:
          case data::token_type::operator_plus_assign:
          case data::token_type::operator_plus_plus:
          case data::token_type::operator_arrow:
          case data::token_type::operator_arrow_star:
          case data::token_type::operator_question_mark:
          case data::token_type::operator_right_brace:
          case data::token_type::operator_right_paren:
          case data::token_type::operator_right_bracket:
          case data::token_type::operator_colon_colon:
          case data::token_type::operator_semicolon:
          case data::token_type::operator_left_shift:
          case data::token_type::operator_left_shift_assign:
          case data::token_type::operator_right_shift:
          case data::token_type::operator_right_shift_assign:
          case data::token_type::operator_star:
          case data::token_type::operator_bitwise_not:
          case data::token_type::operator_star_assign:
          case data::token_type::operator_pound_pound:
            // ignore
            break;
          }
        }

        result.insert(engine().code_completer().code_complete(
            *_env, s_, enabled(data::shell_flag::use_precompiled_headers)));
      }
      catch (...)
      {
        // ignore
      }

      return result;
    }

    const std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>>&
    shell::pragma_handlers() const
    {
      return _pragma_handlers;
    }

    bool shell::stopped() const { return _stopped; }

    void shell::stop() { _stopped = true; }

    iface::environment& shell::env() { return *_env; }

    const iface::environment& shell::env() const { return *_env; }

    void shell::rebuild_environment(const data::cpp_code& content_)
    {
      _env = std::make_unique<core::header_file_environment>(
          try_to_get_shell(engine()), _config.active_shell_config(),
          _internal_dir, _env_filename);

      if (!content_.empty())
      {
        _env->append(content_);
      }
    }

    void shell::rebuild_environment()
    {
      rebuild_environment(_env ? _env->get_all() : data::cpp_code());
    }

    void shell::push_environment() { _environment_stack.push(_env->get_all()); }

    void shell::pop_environment()
    {
      if (_environment_stack.empty())
      {
        throw data::exception("The environment stack is empty.");
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
        displayer_.show_comment(data::text("Environment stack is empty"));
      }
      else if (_environment_stack.size() == 1)
      {
        displayer_.show_comment(data::text("Environment stack has 1 entry"));
      }
      else
      {
        std::ostringstream s;
        s << "Environment stack has " << _environment_stack.size()
          << " entries";
        displayer_.show_comment(data::text(s.str()));
      }
    }

    void shell::run_metaprogram(const data::cpp_code& s_,
                                iface::displayer& displayer_)
    {
      if (_evaluate_metaprograms)
      {
        const data::result r = eval_tmp_formatted(
            *_env, s_, enabled(data::shell_flag::use_precompiled_headers),
            engine().type_shell(), _logger);
        if (_show_cpp_errors || r.successful)
        {
          display(r, displayer_, true);
        }
      }
    }

    void shell::reset_environment()
    {
      rebuild_environment(data::cpp_code());
      _env->append(data::cpp_code(default_env));
    }

    const data::config& shell::get_config() const { return _config; }

    data::config& shell::get_config() { return _config; }

    void shell::line_available(const data::user_input& s_,
                               iface::displayer& displayer_)
    {
      core::null_history h;
      line_available(s_, displayer_, h);
    }

    iface::engine& shell::engine()
    {
      const auto& name = _config.active_shell_config().name;
      const auto i = _engines.find(name);
      if (i == _engines.end())
      {
        return *_engines
                    .insert(std::make_pair(
                        name, _engine_builder(_config.active_shell_config())))
                    .first->second;
      }
      else
      {
        assert(i->second);
        return *i->second;
      }
    }

    boost::filesystem::path shell::env_path() const
    {
      return _internal_dir / _env_filename;
    }

    bool shell::preprocess(iface::displayer& displayer_,
                           const data::cpp_code& exp_,
                           bool process_directives_)
    {
      data::result r = engine().preprocessor_shell().precompile(
          _env->get_all() + data::cpp_code("\n") +
          add_markers(exp_, process_directives_) + data::cpp_code("\n"));

      if (r.successful)
      {
        try
        {
          r.output =
              remove_markers(data::cpp_code(r.output), process_directives_)
                  .value();
        }
        catch (const std::exception& e_)
        {
          make_failure(r, e_.what());
        }
      }

      display(r, displayer_, false);
      return r.successful;
    }

    bool shell::enabled(data::shell_flag flag_) const
    {
      switch (flag_)
      {
      case data::shell_flag::echo:
        return _echo;
      case data::shell_flag::evaluate_metaprograms:
        return _evaluate_metaprograms;
      case data::shell_flag::show_cpp_errors:
        return _show_cpp_errors;
      case data::shell_flag::use_precompiled_headers:
        return _config.active_shell_config().use_precompiled_headers;
      case data::shell_flag::verbose:
        return _config.verbose;
      }
      assert(!"Invalid flag");
    }

    void shell::enabled(data::shell_flag flag_, bool enabled_)
    {
      switch (flag_)
      {
      case data::shell_flag::echo:
        _echo = enabled_;
        break;
      case data::shell_flag::evaluate_metaprograms:
        _evaluate_metaprograms = enabled_;
        break;
      case data::shell_flag::show_cpp_errors:
        _show_cpp_errors = enabled_;
        break;
      case data::shell_flag::use_precompiled_headers:
        _config.active_shell_config().use_precompiled_headers = enabled_;
        rebuild_environment();
        break;
      case data::shell_flag::verbose:
        _config.verbose = enabled_;
        break;
      }
    }

    void shell::switch_to(const data::real_engine_name& engine_)
    {
      const data::shell_config_name config_name =
          get_config().active_shell_config().name;

      data::overridable<data::engine_arguments>& engine_args =
          get_config().active_shell_config().engine;
      const data::overridable<data::engine_arguments> old = engine_args;

      try
      {
        engine_args.set_override();
        _engines.erase(config_name);
        rebuild_environment();

        const data::engine_config old_config = engine().config();

        engine_args.set_override(convert_to(engine_, old_config));
        _engines.erase(config_name);
        rebuild_environment();
      }
      catch (...)
      {
        _engines.erase(get_config().active_shell_config().name);
        engine_args = old;

        rebuild_environment();
        throw;
      }
    }

    const std::vector<data::real_engine_name>& shell::available_engines() const
    {
      return _available_engines;
    }
  }
}
