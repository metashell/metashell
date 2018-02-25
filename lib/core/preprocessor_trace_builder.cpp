// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/metaprogram_builder.hpp>
#include <metashell/preprocessor_trace_builder.hpp>
#include <metashell/wave_context.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <boost/wave/grammars/cpp_chlit_grammar.hpp>
#include <boost/wave/grammars/cpp_defined_grammar.hpp>
#include <boost/wave/grammars/cpp_expression_grammar.hpp>
#include <boost/wave/grammars/cpp_grammar.hpp>
#include <boost/wave/grammars/cpp_intlit_grammar.hpp>
#include <boost/wave/grammars/cpp_predef_macros_grammar.hpp>

#include <cassert>
#include <ctime>
#include <functional>

namespace
{
  metashell::data::cpp_code
  determine_input(metashell::data::cpp_code env_,
                  const boost::optional<metashell::data::cpp_code>& exp_)
  {
    if (exp_)
    {
      return env_ + "\n" + add_markers(*exp_, true) + "\n";
    }
    else
    {
      return env_;
    }
  }

  double now() { return double(std::time(nullptr)); }
}

namespace metashell
{
  preprocessor_trace_builder::preprocessor_trace_builder(
      data::cpp_code env_,
      boost::optional<data::cpp_code> exp_,
      data::metaprogram::mode_t mode_)
    : _env(std::move(env_)),
      _exp(std::move(exp_)),
      _builder(mode_,
               _exp ? *_exp : data::cpp_code("<environment>"),
               data::file_location())
  {
  }

  data::metaprogram
  preprocessor_trace_builder::build(const data::wave_config& config_)
  {
    namespace p = std::placeholders;

    const data::cpp_code input = determine_input(_env, _exp);
    _num_tokens_from_macro_call = 0;
    _macro_loc_stack.clear();

    try
    {
      const char env_path[] = "<stdin>";

      wave_context ctx(input.begin(), input.end(), env_path);

      auto& hooks = ctx.get_hooks();

      hooks.lines_of_env = std::count(_env.begin(), _env.end(), '\n');

      // this value comes from the lines added by determine_input. Since the
      // function adds a suffix as well, I can't think of a way of automatically
      // determining this value.
      hooks.lines_to_ignore_after_env = 3;
      hooks.env_path = env_path;

      hooks.on_macro_expansion_begin =
          std::bind(&preprocessor_trace_builder::on_macro_expansion_begin, this,
                    p::_1, p::_2, p::_3, p::_4);
      hooks.on_rescanning =
          std::bind(&preprocessor_trace_builder::on_rescanning, this, p::_1);
      hooks.on_macro_expansion_end =
          std::bind(&preprocessor_trace_builder::on_macro_expansion_end, this,
                    p::_1, p::_2);
      hooks.on_token_generated = std::bind(
          &preprocessor_trace_builder::on_token_generated, this, p::_1, p::_2);
      hooks.on_token_skipped = std::bind(
          &preprocessor_trace_builder::on_token_skipped, this, p::_1, p::_2);
      hooks.on_include_begin = std::bind(
          &preprocessor_trace_builder::on_include_begin, this, p::_1, p::_2);
      hooks.on_include_end =
          std::bind(&preprocessor_trace_builder::on_include_end, this);
      hooks.on_define = std::bind(&preprocessor_trace_builder::on_define, this,
                                  p::_1, p::_2, p::_3, p::_4);
      hooks.on_undefine = std::bind(
          &preprocessor_trace_builder::on_undefine, this, p::_1, p::_2);

      hooks.on_conditional = std::bind(
          &preprocessor_trace_builder::on_conditional, this, p::_1, p::_2);

      hooks.on_evaluated_conditional_expression = std::bind(
          &preprocessor_trace_builder::on_evaluated_conditional_expression,
          this, p::_1);

      hooks.on_else =
          std::bind(&preprocessor_trace_builder::on_else, this, p::_1);

      hooks.on_endif =
          std::bind(&preprocessor_trace_builder::on_endif, this, p::_1);

      hooks.on_error =
          std::bind(&preprocessor_trace_builder::on_error, this, p::_1, p::_2);

      hooks.on_line = std::bind(
          &preprocessor_trace_builder::on_line, this, p::_1, p::_2, p::_3);

      apply(ctx, config_);

      std::ostringstream s;
      display(s, ctx, config_.ignore_macro_redefinition);
      const data::cpp_code output_code(s.str());
      _builder.handle_event(
          data::event_details<data::event_kind::evaluation_end>{
              _exp ? remove_markers(output_code, true) : output_code});
    }
    catch (const boost::wave::cpp_exception& error_)
    {
      _builder.handle_event(
          data::event_details<data::event_kind::evaluation_end>{
              data::type_or_code_or_error::make_error(to_string(error_))});
    }
    catch (const std::exception& error_)
    {
      _builder.handle_event(
          data::event_details<data::event_kind::evaluation_end>{
              data::type_or_code_or_error::make_error(error_.what())});
    }

    return _builder.get_metaprogram();
  }

  void preprocessor_trace_builder::on_macro_expansion_begin(
      const data::cpp_code& name_,
      const boost::optional<std::vector<data::cpp_code>>& args_,
      const data::file_location& point_of_event_,
      const data::file_location& source_location_)
  {
    if (_macro_loc_stack.empty())
    {
      _last_macro_call_loc = point_of_event_;
    }
    _macro_loc_stack.push_back(point_of_event_);
    _builder.handle_event(
        data::event_details<data::event_kind::macro_expansion>{
            name_, args_, point_of_event_, source_location_, now()});
  }

  void preprocessor_trace_builder::on_rescanning(const data::cpp_code& c_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::rescanning>{c_, now()});
  }

  void
  preprocessor_trace_builder::on_macro_expansion_end(const data::cpp_code& c_,
                                                     int num_tokens_)
  {
    assert(!_macro_loc_stack.empty());

    _builder.handle_event(data::event_details<data::event_kind::expanded_code>{
        c_, _macro_loc_stack.back(), now()});
    _builder.handle_event(
        data::event_details<data::event_kind::macro_expansion_end>{now()});
    _num_tokens_from_macro_call = num_tokens_;
    _macro_loc_stack.pop_back();
  }

  void preprocessor_trace_builder::on_token_generated(
      const data::token& t_, const data::file_location& source_location_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::generated_token>{
            t_, _num_tokens_from_macro_call > 0 ? _last_macro_call_loc :
                                                  source_location_,
            source_location_, now()});
    if (_num_tokens_from_macro_call > 0)
    {
      --_num_tokens_from_macro_call;
    }
  }

  void preprocessor_trace_builder::on_token_skipped(
      const data::token& t_, const data::file_location& source_location_)
  {
    _builder.handle_event(data::event_details<data::event_kind::skipped_token>{
        t_, source_location_, now()});
  }

  void preprocessor_trace_builder::on_include_begin(
      const data::include_argument& arg_,
      const data::file_location& point_of_event_)
  {
    switch (arg_.type)
    {
    case data::include_type::sys:
      _builder.handle_event(data::event_details<data::event_kind::sys_include>{
          arg_.path, point_of_event_, now()});
      break;
    case data::include_type::quote:
      _builder.handle_event(
          data::event_details<data::event_kind::quote_include>{
              arg_.path, point_of_event_, now()});
      break;
    }
  }

  void preprocessor_trace_builder::on_include_end()
  {
    _builder.handle_event(
        data::event_details<data::event_kind::include_end>{now()});
  }

  void preprocessor_trace_builder::on_define(
      const data::cpp_code& name_,
      const boost::optional<std::vector<data::cpp_code>>& args_,
      const data::cpp_code& body_,
      const data::file_location& point_of_event_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::macro_definition>{
            name_, args_, body_, point_of_event_, now()});
  }

  void preprocessor_trace_builder::on_undefine(
      const data::cpp_code& name_, const data::file_location& point_of_event_)
  {
    _builder.handle_event(data::event_details<data::event_kind::macro_deletion>{
        name_, point_of_event_, now()});
  }

  void preprocessor_trace_builder::on_conditional(
      const data::cpp_code& expression_,
      const data::file_location& point_of_event_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::preprocessing_condition>{
            expression_, point_of_event_, now()});
  }

  void
  preprocessor_trace_builder::on_evaluated_conditional_expression(bool result_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::preprocessing_condition_result>{
            result_, now()});
  }

  void preprocessor_trace_builder::on_else(
      const data::file_location& point_of_event_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::preprocessing_else>{
            point_of_event_, now()});
  }

  void preprocessor_trace_builder::on_endif(
      const data::file_location& point_of_event_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::preprocessing_endif>{
            point_of_event_, now()});
  }

  void preprocessor_trace_builder::on_error(
      const std::string& message_, const data::file_location& point_of_event_)
  {
    _builder.handle_event(
        data::event_details<data::event_kind::error_directive>{
            message_, point_of_event_, now()});
  }

  void preprocessor_trace_builder::on_line(
      const data::cpp_code& arg_,
      const data::file_location& point_of_event_,
      const data::file_location& source_location_)
  {
    _builder.handle_event(data::event_details<data::event_kind::line_directive>{
        arg_, point_of_event_, source_location_, now()});
  }
}
