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

#include <metashell/engine/wave/empty_environment.hpp>
#include <metashell/engine/wave/parse_config.hpp>

#include <metashell/engine/clang/binary.hpp>
#include <metashell/engine/clang/header_discoverer.hpp>
#include <metashell/engine/clang/macro_discovery.hpp>

#include <metashell/core/wave_token.hpp>

#include <metashell/data/wave_arg_parser.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <algorithm>
#include <set>
#include <sstream>
#include <vector>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      namespace
      {
        class macro_definition_collector
            : public boost::wave::context_policies::eat_whitespace<
                  core::wave_token>
        {
        public:
          explicit macro_definition_collector(std::vector<std::string>& out_)
            : _out(out_)
          {
          }

          template <class Context,
                    class Token,
                    class Parameters,
                    class Definition>
          void defined_macro(const Context&,
                             const Token& macro_name_,
                             bool is_functionlike_,
                             const Parameters& parameters_,
                             const Definition& definition_,
                             bool is_predefined_)
          {
            if (!is_predefined_ &&
                _blacklist.find(macro_name_.get_value()) == _blacklist.end())
            {
              std::ostringstream s;
              s << macro_name_.get_value();
              if (is_functionlike_)
              {
                s << "(";
                bool first = true;
                for (const auto& param : parameters_)
                {
                  if (first)
                  {
                    first = false;
                  }
                  else
                  {
                    s << ", ";
                  }
                  s << param;
                }
                s << ")";
              }
              s << "=";
              std::transform(
                  definition_.begin(), definition_.end(),
                  std::ostream_iterator<typename Token::string_type>(s),
                  [](const Token& t_) { return t_.get_value(); });
              _out.push_back(s.str());
            }
          }

          void blacklist(core::wave_token::string_type name_)
          {
            _blacklist.insert(std::move(name_));
          }

        private:
          std::vector<std::string>& _out;
          std::set<core::wave_token::string_type> _blacklist;
        };

        std::vector<std::string>
        clang_macros(const clang::binary& cbin_,
                     const boost::filesystem::path& internal_dir_)
        {
          // Need to have __has_feature(cxx_decltype) to return true in order
          // to be able to parse libcxx headers. They define decltype as a macro
          // otherwise, and call it with template expression that contains
          // multiple preprocessor parameters.
          std::vector<std::string> result{
              "__has_include_next(_)=0",
              "__metashell_has_feature_impl__cxx_decltype=1",
              "__has_feature(x)=__metashell_has_feature_impl__ ## x"};

          empty_environment env(internal_dir_);
          const data::cpp_code defines =
              clang::macro_discovery(cbin_).macros(env);

          macro_definition_collector def_collector(result);
          boost::wave::context<
              std::string::const_iterator,
              boost::wave::cpplexer::lex_iterator<core::wave_token>,
              boost::wave::iteration_context_policies::load_file_to_string,
              macro_definition_collector>
              ctx(begin(defines), end(defines), "<macros>", def_collector);

          ctx.set_language(boost::wave::language_support(
              boost::wave::support_cpp0x |
              boost::wave::support_option_convert_trigraphs |
              boost::wave::support_option_long_long |
              boost::wave::support_option_emit_line_directives |
              boost::wave::support_option_include_guard_detection |
              boost::wave::support_option_emit_pragma_directives |
              boost::wave::support_option_insert_whitespace));
          for (auto i = ctx.macro_names_begin(); i != ctx.macro_names_end();
               ++i)
          {
            def_collector.blacklist(*i);
          }

          const auto e = ctx.end();
          auto i = ctx.begin();
          bool iterate = true;
          while (iterate)
          {
            try
            {
              if (i == e)
              {
                iterate = false;
              }
              else
              {
                ++i;
              }
            }
            catch (const boost::wave::cpp_exception& error_)
            {
              if (!error_.is_recoverable())
              {
                throw;
              }
            }
          }

          return result;
        }

        data::engine_config
        internal_clang_config(const data::executable_path& metashell_binary_,
                              const boost::filesystem::path& internal_dir_,
                              iface::environment_detector& env_detector_,
                              iface::displayer& displayer_,
                              core::logger* logger_)
        {
          data::engine_config result;
          const data::command_line_argument_list extra_clang_args;
          if (const auto clang_path = clang::find_clang_nothrow(
                  true, extra_clang_args, metashell_binary_,
                  data::real_engine_name::internal, env_detector_, displayer_,
                  logger_))
          {
            const clang::binary cbin(true, *clang_path, extra_clang_args,
                                     internal_dir_, env_detector_, logger_);

            clang::header_discoverer header_discoverer(cbin);

            result.includes.isystem =
                header_discoverer.include_path(data::include_type::sys);
            result.includes.iquote =
                header_discoverer.include_path(data::include_type::quote);
            result.macros = clang_macros(cbin, internal_dir_);
          }
          return result;
        }

        template <class T, class U>
        void push_back_times(int times_, T& out_, const U& value_)
        {
          for (int i = 0; i < times_; ++i)
          {
            out_.push_back(value_);
          }
        }
      }

      data::wave_config
      parse_config(bool use_templight_headers_,
                   const data::command_line_argument_list& args_,
                   const data::executable_path& metashell_binary_,
                   const boost::filesystem::path& internal_dir_,
                   iface::environment_detector& env_detector_,
                   iface::displayer& displayer_,
                   core::logger* logger_)
      {
        data::engine_config defaults =
            use_templight_headers_ ?
                internal_clang_config(metashell_binary_, internal_dir_,
                                      env_detector_, displayer_, logger_) :
                data::engine_config();
        data::wave_arg_parser parser(use_templight_headers_);
        parser.parse(
            args_, std::move(defaults.includes), std::move(defaults.macros));
        return parser.result();
      }
    }
  }
}
