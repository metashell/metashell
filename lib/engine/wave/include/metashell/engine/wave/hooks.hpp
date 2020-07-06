#ifndef METASHELL_ENGINE_WAVE_HOOKS_HPP
#define METASHELL_ENGINE_WAVE_HOOKS_HPP

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

#include <metashell/core/wave_token.hpp>

#include <metashell/data/counter.hpp>
#include <metashell/data/cpp_code.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/include_argument.hpp>
#include <metashell/data/token.hpp>

#include <boost/wave.hpp>

#include <boost/filesystem/path.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <optional>
#include <set>
#include <sstream>
#include <vector>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      class hooks : public boost::wave::context_policies::eat_whitespace<
                        core::wave_token>
      {
      public:
        int lines_of_env = 0;
        int lines_to_ignore_after_env = 0;
        boost::filesystem::path env_path;

        std::function<void(const data::cpp_code&,
                           const std::optional<std::vector<data::cpp_code>>&,
                           const data::file_location&,
                           const data::file_location&)>
            on_macro_expansion_begin;

        std::function<void(data::cpp_code)> on_rescanning;

        std::function<void(data::cpp_code, int)> on_macro_expansion_end;

        std::function<void(data::token, data::file_location)>
            on_token_generated;

        std::function<void(data::token, data::file_location)> on_token_skipped;

        std::function<void(data::include_argument, data::file_location)>
            on_include_begin;
        std::function<void()> on_include_end;

        std::function<void(data::cpp_code,
                           std::optional<std::vector<data::cpp_code>>,
                           data::cpp_code,
                           data::file_location)>
            on_define;
        std::function<void(data::cpp_code, data::file_location)> on_undefine;

        std::function<void(data::cpp_code, data::file_location)> on_conditional;

        std::function<void(bool)> on_evaluated_conditional_expression;

        std::function<void(data::file_location)> on_else;
        std::function<void(data::file_location)> on_endif;

        std::function<void(std::string, data::file_location)> on_error;

        std::function<void(
            data::cpp_code, data::file_location, data::file_location)>
            on_line;

        hooks() : _included_files(nullptr) {}

        explicit hooks(std::set<boost::filesystem::path>& included_files_)
          : _included_files(&included_files_)
        {
        }

        template <typename ContextT>
        void opened_include_file(const ContextT&,
                                 const std::string&,
                                 const std::string& absname_,
                                 bool is_system_include_)
        {
          if (_included_files)
          {
            _included_files->insert(absname_);
          }
          if (on_include_begin)
          {
            on_include_begin(data::include_argument(
                                 is_system_include_ ? data::include_type::sys :
                                                      data::include_type::quote,
                                 absname_),
                             _last_directive_location);
          }
          ++_include_depth;
        }

        template <typename ContextT>
        void returning_from_include_file(const ContextT&)
        {
          if (on_include_end)
          {
            on_include_end();
          }
          assert(!_include_depth.empty());
          --_include_depth;
        }

        template <typename ContextT,
                  typename TokenT,
                  typename ContainerT,
                  typename IteratorT>
        bool
        expanding_function_like_macro(const ContextT&,
                                      const TokenT& macrodef_,
                                      const std::vector<TokenT>&,
                                      const ContainerT&,
                                      const TokenT& macrocall_,
                                      const std::vector<ContainerT>& arguments_,
                                      const IteratorT&,
                                      const IteratorT&)
        {
          if (on_macro_expansion_begin)
          {
            std::vector<data::cpp_code> args;
            args.reserve(arguments_.size());
            std::transform(arguments_.begin(), arguments_.end(),
                           std::back_inserter(args),
                           &tokens_to_code<ContainerT>);

            const auto name = token_to_code(macrodef_);
            const auto point_of_event = to_file_location(macrocall_);
            const auto source_location = to_file_location(macrodef_);
            trigger_event([this, name, args, point_of_event, source_location] {
              this->on_macro_expansion_begin(
                  name, args, point_of_event, source_location);
            });
          }
          return false;
        }

        template <typename ContextT, typename TokenT, typename ContainerT>
        bool expanding_object_like_macro(const ContextT&,
                                         const TokenT& macrodef_,
                                         const ContainerT&,
                                         const TokenT& macrocall_)
        {
          if (on_macro_expansion_begin)
          {
            const auto name = token_to_code(macrodef_);
            const auto point_of_event = to_file_location(macrocall_);
            const auto source_location = to_file_location(macrodef_);
            trigger_event([this, name, point_of_event, source_location] {
              this->on_macro_expansion_begin(
                  name, std::nullopt, point_of_event, source_location);
            });
          }
          return false;
        }

        template <typename ContextT, typename ContainerT>
        void expanded_macro(const ContextT&, const ContainerT& result_)
        {
          if (on_rescanning)
          {
            const auto result = tokens_to_code(result_);
            trigger_event([this, result] { this->on_rescanning(result); });
          }
        }

        template <typename ContextT, typename ContainerT>
        void rescanned_macro(const ContextT&, const ContainerT& result_)
        {
          if (on_macro_expansion_end)
          {
            const auto result = tokens_to_code(result_);
            const auto num_tokens = result_.size();
            trigger_event([this, result, num_tokens] {
              this->on_macro_expansion_end(result, num_tokens);
            });
          }
        }

        template <typename ContextT, typename TokenT>
        const TokenT& generated_token(const ContextT&, const TokenT& t_)
        {
          if (!IS_CATEGORY(t_, boost::wave::token_category::EOFTokenType) &&
              on_token_generated)
          {
            on_token_generated(
                core::token_from_wave_token(t_), to_file_location(t_));
          }
          return t_;
        }

        template <typename ContextT, typename TokenT>
        bool found_directive(const ContextT&, const TokenT& directive_)
        {
          _last_directive_location = to_file_location(directive_);
          const auto directive = token_to_code(directive_);
          if (directive == "#if" || directive == "#elif" ||
              directive == "#ifdef" || directive == "#ifndef")
          {
            _event_queue = std::vector<std::function<void()>>();
          }
          else if (directive == "#else")
          {
            if (on_else)
            {
              on_else(_last_directive_location);
            }
          }
          else if (directive == "#endif")
          {
            if (on_endif)
            {
              on_endif(_last_directive_location);
            }
          }
          return false;
        }

        template <typename ContextT,
                  typename TokenT,
                  typename ParametersT,
                  typename DefinitionT>
        void defined_macro(const ContextT&,
                           const TokenT& macro_name_,
                           bool is_functionlike_,
                           const ParametersT& parameters_,
                           const DefinitionT& definition_,
                           bool)
        {
          if (on_define)
          {
            std::optional<std::vector<data::cpp_code>> args;
            if (is_functionlike_)
            {
              args = std::vector<data::cpp_code>();
              args->reserve(parameters_.size());
              std::transform(parameters_.begin(), parameters_.end(),
                             std::back_inserter(*args), &token_to_code<TokenT>);
            }

            on_define(token_to_code(macro_name_), args,
                      tokens_to_code(definition_),
                      to_file_location(macro_name_));
          }
        }

        template <typename ContextT, typename TokenT>
        void undefined_macro(const ContextT&, const TokenT& macro_name_)
        {
          if (on_undefine)
          {
            on_undefine(
                token_to_code(macro_name_), to_file_location(macro_name_));
          }
        }

        template <typename ContextT, typename TokenT, typename ContainerT>
        bool evaluated_conditional_expression(const ContextT&,
                                              const TokenT& directive_,
                                              const ContainerT& expression_,
                                              bool expression_value_)
        {
          if (on_conditional)
          {
            on_conditional(token_to_code(directive_) + data::cpp_code(" ") +
                               tokens_to_code(expression_),
                           to_file_location(directive_));
          }
          flush_event_queue();

          if (on_evaluated_conditional_expression)
          {
            on_evaluated_conditional_expression(expression_value_);
          }
          return false;
        }

        template <typename ContextT, typename TokenT>
        void skipped_token(const ContextT&, const TokenT& token_)
        {
          if (on_token_skipped)
          {
            on_token_skipped(
                core::token_from_wave_token(token_), to_file_location(token_));
          }
        }

        template <typename ContextT, typename ContainerT>
        bool found_error_directive(const ContextT&, const ContainerT& message_)
        {
          if (on_error)
          {
            on_error(tokens_to_string(message_), _last_directive_location);
          }

          for (; !_include_depth.empty(); --_include_depth)
          {
            if (on_include_end)
            {
              on_include_end();
            }
          }

          return false;
        }

        template <typename ContextT, typename ContainerT>
        void found_line_directive(const ContextT&,
                                  const ContainerT& arguments_,
                                  unsigned int line_,
                                  const std::string& filename_)
        {
          if (on_line && arguments_.begin() != arguments_.end())
          {
            on_line(tokens_to_code(arguments_),
                    to_file_location(*arguments_.begin()),
                    data::file_location(filename_, line_, 1));
          }
        }

      private:
        std::set<boost::filesystem::path>* _included_files;
        data::file_location _last_directive_location;
        std::optional<std::vector<std::function<void()>>> _event_queue;
        data::counter _include_depth;

        template <class String>
        static std::string to_std_string(const String& s_)
        {
          std::ostringstream result;
          result << s_;
          return result.str();
        }

        template <class Token>
        static std::string token_to_string(const Token& token_)
        {
          return to_std_string(token_.get_value());
        }

        template <class Token>
        static data::cpp_code token_to_code(const Token& token_)
        {
          return data::cpp_code(token_to_string(token_));
        }

        template <class ContainerT>
        static std::string tokens_to_string(const ContainerT& tokens_)
        {
          std::ostringstream result;
          for (const auto& t : tokens_)
          {
            result << t.get_value();
          }
          return result.str();
        }

        template <class ContainerT>
        static data::cpp_code tokens_to_code(const ContainerT& tokens_)
        {
          return data::cpp_code(tokens_to_string(tokens_));
        }

        template <class Token>
        data::file_location to_file_location(const Token& token_)
        {
          const auto pos = token_.get_position();
          const std::string fn = to_std_string(pos.get_file());
          int line = pos.get_line();
          if (fn == env_path && line > lines_of_env)
          {
            line -= lines_to_ignore_after_env;
          }
          return data::file_location(
              boost::filesystem::path(fn).filename() == "<stdin>" ? "<stdin>" :
                                                                    fn,
              line, pos.get_column());
        }

        void trigger_event(std::function<void()> event_)
        {
          if (_event_queue)
          {
            _event_queue->push_back(event_);
          }
          else
          {
            event_();
          }
        }

        void flush_event_queue()
        {
          if (_event_queue)
          {
            for (const auto& event : *_event_queue)
            {
              event();
            }
            _event_queue = std::nullopt;
          }
        }
      };
    }
  }
}

#endif
