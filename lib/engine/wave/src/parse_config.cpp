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
          explicit macro_definition_collector(
              std::vector<data::macro_definition>& out_)
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
              _out.emplace_back(s.str());
            }
          }

          void blacklist(core::wave_token::string_type name_)
          {
            _blacklist.insert(std::move(name_));
          }

        private:
          std::vector<data::macro_definition>& _out;
          std::set<core::wave_token::string_type> _blacklist;
        };

        std::vector<data::macro_definition>
        clang_macros(const clang::binary& cbin_,
                     const boost::filesystem::path& internal_dir_)
        {
          // Need to have __has_feature(cxx_decltype) to return true in order
          // to be able to parse libcxx headers. They define decltype as a macro
          // otherwise, and call it with template expression that contains
          // multiple preprocessor parameters.
          std::vector<data::macro_definition> result{
              data::macro_definition("__has_include_next(_)=0"),
              data::macro_definition(
                  "__metashell_has_feature_impl__cxx_decltype=1"),
              data::macro_definition(
                  "__has_feature(x)=__metashell_has_feature_impl__ ## x")};

          empty_environment env(internal_dir_);
          const data::cpp_code defines =
              clang::macro_discovery(cbin_).macros(env);

          macro_definition_collector def_collector(result);
          boost::wave::context<
              std::string::const_iterator,
              boost::wave::cpplexer::lex_iterator<core::wave_token>,
              boost::wave::iteration_context_policies::load_file_to_string,
              macro_definition_collector>
              ctx(defines.begin(), defines.end(), "<macros>", def_collector);

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

        std::vector<data::macro_definition>
        internal_clang_macros(const data::executable_path& metashell_binary_,
                              const boost::filesystem::path& internal_dir_,
                              iface::environment_detector& env_detector_,
                              iface::displayer& displayer_,
                              core::logger* logger_)
        {
          data::engine_config result;
          const data::command_line_argument_list extra_clang_args;
          if (const auto clang_path = clang::find_clang_nothrow(
                  true,
                  data::engine_arguments{
                      extra_clang_args, data::real_engine_name::internal},
                  metashell_binary_, env_detector_, displayer_, logger_))
          {
            return clang_macros(
                clang::binary(true, *clang_path, extra_clang_args,
                              boost::filesystem::path(), internal_dir_,
                              env_detector_, logger_),
                internal_dir_);
          }
          return {};
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
        data::wave_arg_parser parser(use_templight_headers_);
        parser.parse(args_, use_templight_headers_ ?
                                internal_clang_macros(
                                    metashell_binary_, internal_dir_,
                                    env_detector_, displayer_, logger_) :
                                std::vector<data::macro_definition>());
        return parser.result();
      }
    }
  }
}
