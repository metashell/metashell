// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/wave/trace_impl.hpp>

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

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      namespace
      {
        const std::string& env_path()
        {
          static const std::string value("<stdin>");
          return value;
        }

        data::cpp_code
        determine_input(const data::cpp_code& env_,
                        const std::optional<data::cpp_code>& exp_)
        {
          if (exp_)
          {
            return env_ + data::cpp_code("\n") + add_markers(*exp_, true) +
                   data::cpp_code("\n");
          }
          else
          {
            return env_;
          }
        }

        double now() { return double(std::time(nullptr)); }
      }

      trace_impl::trace_impl(
          const data::cpp_code& env_,
          const std::optional<data::cpp_code>& exp_,
          const data::wave_config& config_,
          const std::vector<boost::filesystem::path>& system_includes_)
        : _full_trace(exp_),
          _ignore_macro_redefinition(config_.ignore_macro_redefinition),
          _input(determine_input(env_, exp_)),
          _num_tokens_from_macro_call(0),
          _ctx(_input.begin(), _input.end(), env_path().c_str()),
          _pos(std::nullopt)
      {
        namespace p = std::placeholders;

        auto& hooks = _ctx.get_hooks();

        hooks.lines_of_env = count(_env, '\n');

        // this value comes from the lines added by determine_input. Since the
        // function adds a suffix as well, I can't think of a way of
        // automatically
        // determining this value.
        hooks.lines_to_ignore_after_env = 3;
        hooks.env_path = env_path();

        hooks.on_macro_expansion_begin =
            std::bind(&trace_impl::on_macro_expansion_begin, this, p::_1, p::_2,
                      p::_3, p::_4);
        hooks.on_rescanning =
            std::bind(&trace_impl::on_rescanning, this, p::_1);
        hooks.on_macro_expansion_end =
            std::bind(&trace_impl::on_macro_expansion_end, this, p::_1, p::_2);
        hooks.on_token_generated =
            std::bind(&trace_impl::on_token_generated, this, p::_1, p::_2);
        hooks.on_token_skipped =
            std::bind(&trace_impl::on_token_skipped, this, p::_1, p::_2);
        hooks.on_include_begin =
            std::bind(&trace_impl::on_include_begin, this, p::_1, p::_2);
        hooks.on_include_end = std::bind(&trace_impl::on_include_end, this);
        hooks.on_define =
            std::bind(&trace_impl::on_define, this, p::_1, p::_2, p::_3, p::_4);
        hooks.on_undefine =
            std::bind(&trace_impl::on_undefine, this, p::_1, p::_2);

        hooks.on_conditional =
            std::bind(&trace_impl::on_conditional, this, p::_1, p::_2);

        hooks.on_evaluated_conditional_expression = std::bind(
            &trace_impl::on_evaluated_conditional_expression, this, p::_1);

        hooks.on_else = std::bind(&trace_impl::on_else, this, p::_1);

        hooks.on_endif = std::bind(&trace_impl::on_endif, this, p::_1);

        hooks.on_error = std::bind(&trace_impl::on_error, this, p::_1, p::_2);

        hooks.on_line =
            std::bind(&trace_impl::on_line, this, p::_1, p::_2, p::_3);

        wave::apply(_ctx, config_, system_includes_);

        _pos = _ctx.begin();
      }

      std::optional<data::event_data> trace_impl::next()
      {
        while (_events.empty() && _pos)
        {
          try
          {
            if (!display_step(
                    _output, *_pos, _ctx.end(), _ignore_macro_redefinition))
            {
              data::cpp_code output_code(_output.str());
              _events.emplace_back(
                  data::event_details<data::event_kind::evaluation_end>{
                      {_full_trace ? remove_markers(output_code, true) :
                                     std::move(output_code)}});
              _pos = std::nullopt;
            }
          }
          catch (const boost::wave::cpp_exception& error_)
          {
            _events.emplace_back(
                data::event_details<data::event_kind::evaluation_end>{
                    {data::type_or_code_or_error::make_error(
                        to_string(error_))}});
            _pos = std::nullopt;
          }
          catch (const std::exception& error_)
          {
            _events.emplace_back(
                data::event_details<data::event_kind::evaluation_end>{
                    {data::type_or_code_or_error::make_error(error_.what())}});
            _pos = std::nullopt;
          }
        }

        std::optional<data::event_data> result;
        if (!_events.empty())
        {
          result = std::move(_events.front());
          _events.pop_front();
        }
        return result;
      }

      void trace_impl::record_point_of_event(
          const data::file_location& point_of_event_)
      {
        if (_macro_loc_stack.empty())
        {
          _point_of_event = point_of_event_;
        }
      }

      void trace_impl::on_macro_expansion_begin(
          const data::cpp_code& name_,
          const std::optional<std::vector<data::cpp_code>>& args_,
          const data::file_location& point_of_event_,
          const data::file_location& source_location_)
      {
        record_point_of_event(point_of_event_);
        _macro_loc_stack.push_back(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::macro_expansion>{
                {name_, args_, point_of_event_, source_location_}, now()});
      }

      void trace_impl::on_rescanning(const data::cpp_code& c_)
      {
        _events.emplace_back(data::event_details<data::event_kind::rescanning>{
            {c_, _point_of_event}, now()});
      }

      void trace_impl::on_macro_expansion_end(const data::cpp_code& c_,
                                              int num_tokens_)
      {
        assert(!_macro_loc_stack.empty());

        _events.emplace_back(
            data::event_details<data::event_kind::expanded_code>{
                {c_, _macro_loc_stack.back()}, now()});
        _events.emplace_back(
            data::event_details<data::event_kind::rescanning_end>{{}, now()});
        _events.emplace_back(
            data::event_details<data::event_kind::macro_expansion_end>{
                {}, now()});
        _num_tokens_from_macro_call = num_tokens_;
        _macro_loc_stack.pop_back();
      }

      void trace_impl::on_token_generated(
          const data::token& t_, const data::file_location& source_location_)
      {
        _events.emplace_back(
            data::event_details<data::event_kind::generated_token>{
                {t_, _num_tokens_from_macro_call > 0 ? _point_of_event :
                                                       source_location_,
                 source_location_},
                now()});
        if (_num_tokens_from_macro_call > 0)
        {
          --_num_tokens_from_macro_call;
        }
      }

      void
      trace_impl::on_token_skipped(const data::token& t_,
                                   const data::file_location& source_location_)
      {
        record_point_of_event(source_location_);
        _events.emplace_back(
            data::event_details<data::event_kind::skipped_token>{
                {t_, source_location_}, now()});
      }

      void
      trace_impl::on_include_begin(const data::include_argument& arg_,
                                   const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        switch (arg_.type)
        {
        case data::include_type::sys:
          _events.emplace_back(
              data::event_details<data::event_kind::sys_include>{
                  {arg_.path, point_of_event_}, now()});
          break;
        case data::include_type::quote:
          _events.emplace_back(
              data::event_details<data::event_kind::quote_include>{
                  {arg_.path, point_of_event_}, now()});
          break;
        }
      }

      void trace_impl::on_include_end()
      {
        _events.emplace_back(
            data::event_details<data::event_kind::include_end>{{}, now()});
      }

      void trace_impl::on_define(
          const data::cpp_code& name_,
          const std::optional<std::vector<data::cpp_code>>& args_,
          const data::cpp_code& body_,
          const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::macro_definition>{
                {name_, args_, body_, point_of_event_}, now()});
      }

      void trace_impl::on_undefine(const data::cpp_code& name_,
                                   const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::macro_deletion>{
                {name_, point_of_event_}, now()});
      }

      void
      trace_impl::on_conditional(const data::cpp_code& expression_,
                                 const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::preprocessing_condition>{
                {expression_, point_of_event_}, now()});
      }

      void trace_impl::on_evaluated_conditional_expression(bool result_)
      {
        _events.emplace_back(data::event_details<
                             data::event_kind::preprocessing_condition_result>{
            {result_, _point_of_event}, now()});
        _events.emplace_back(
            data::event_details<data::event_kind::preprocessing_condition_end>{
                {}, now()});
      }

      void trace_impl::on_else(const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::preprocessing_else>{
                {point_of_event_}, now()});
      }

      void trace_impl::on_endif(const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::preprocessing_endif>{
                {point_of_event_}, now()});
      }

      void trace_impl::on_error(const std::string& message_,
                                const data::file_location& point_of_event_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::error_directive>{
                {message_, point_of_event_}, now()});
      }

      void trace_impl::on_line(const data::cpp_code& arg_,
                               const data::file_location& point_of_event_,
                               const data::file_location& source_location_)
      {
        record_point_of_event(point_of_event_);
        _events.emplace_back(
            data::event_details<data::event_kind::line_directive>{
                {arg_, point_of_event_, source_location_}, now()});
      }
    }
  }
}
