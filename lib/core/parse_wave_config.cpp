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

#include <metashell/clang_binary.hpp>
#include <metashell/empty_environment.hpp>
#include <metashell/header_discoverer_clang.hpp>
#include <metashell/macro_discovery_clang.hpp>
#include <metashell/parse_wave_config.hpp>
#include <metashell/wave_token.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include <boost/algorithm/string/join.hpp>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>

namespace
{
  class macro_definition_collector
      : public boost::wave::context_policies::eat_whitespace<
            metashell::wave_token>
  {
  public:
    explicit macro_definition_collector(std::vector<std::string>& out_)
      : _out(out_)
    {
    }

    template <class Context, class Token, class Parameters, class Definition>
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
        std::transform(definition_.begin(), definition_.end(),
                       std::ostream_iterator<typename Token::string_type>(s),
                       [](const Token& t_) { return t_.get_value(); });
        _out.push_back(s.str());
      }
    }

    void blacklist(metashell::wave_token::string_type name_)
    {
      _blacklist.insert(std::move(name_));
    }

  private:
    std::vector<std::string>& _out;
    std::set<metashell::wave_token::string_type> _blacklist;
  };

  std::vector<std::string>
  clang_macros(const metashell::clang_binary& cbin_,
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

    metashell::empty_environment env(internal_dir_);
    const metashell::data::cpp_code defines =
        metashell::macro_discovery_clang(cbin_).macros(env);

    macro_definition_collector def_collector(result);
    boost::wave::context<
        std::string::const_iterator,
        boost::wave::cpplexer::lex_iterator<metashell::wave_token>,
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
    for (auto i = ctx.macro_names_begin(); i != ctx.macro_names_end(); ++i)
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

  metashell::data::wave_config
  internal_clang_config(const std::string& metashell_binary_,
                        const boost::filesystem::path& internal_dir_,
                        metashell::iface::environment_detector& env_detector_,
                        metashell::iface::displayer& displayer_,
                        metashell::logger* logger_)
  {
    const std::vector<std::string> extra_clang_args;

    const metashell::clang_binary cbin(
        true, find_clang(true, extra_clang_args, metashell_binary_, "internal",
                         env_detector_, displayer_, logger_),
        extra_clang_args, internal_dir_, env_detector_, logger_);

    metashell::header_discoverer_clang header_discoverer(cbin);

    metashell::data::wave_config result;
    result.includes.sys =
        header_discoverer.include_path(metashell::data::include_type::sys);
    result.includes.quote =
        header_discoverer.include_path(metashell::data::include_type::quote);
    result.macros = clang_macros(cbin, internal_dir_);
    result.standard = metashell::data::wave_standard::cpp11;
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

  int total_count(const boost::program_options::variables_map& vm_,
                  const std::vector<std::string>& args_)
  {
    return std::accumulate(args_.begin(), args_.end(), 0,
                           [&vm_](int sum_, const std::string& arg_) {
                             return sum_ + vm_.count(arg_);
                           });
  }

  std::vector<std::string>
  collect_args(const boost::program_options::variables_map& vm_,
               const std::vector<std::string>& args_)
  {
    std::vector<std::string> result;
    result.reserve(total_count(vm_, args_));
    for (const std::string& arg : args_)
    {
      push_back_times(vm_.count(arg), result, "--" + arg);
    }
    return result;
  }

  void validate(const boost::program_options::variables_map& vm_)
  {
    const std::vector<std::string> stds{"c99", "c++11"};
    if (total_count(vm_, stds) > 1)
    {
      throw std::runtime_error(
          "Multiple standards (" +
          boost::algorithm::join(collect_args(vm_, stds), " ") + ") specified");
    }
  }

  void append(std::vector<boost::filesystem::path>& out_,
              const std::vector<boost::filesystem::path>& extension_)
  {
    out_.insert(out_.end(), extension_.begin(), extension_.end());
  }

  boost::program_options::options_description
  wave_options(bool use_templight_headers_,
               std::vector<boost::filesystem::path>& quote_includes_,
               std::vector<boost::filesystem::path>& sys_includes_,
               std::vector<std::string>& macros_)
  {
    using boost::program_options::value;

    const std::string nostdinc_help =
        use_templight_headers_ ?
            "don't add standard headers to the include path" :
            "ignored (accepted to be compatible with the `wave` engine)";

    boost::program_options::options_description desc("Wave options");
    // clang-format off
    desc.add_options()
      ("include,I", value(&quote_includes_),
        "specify an additional include directory")
      ("sysinclude,S", value(&sys_includes_),
        "specify an additional system include directory")
      ("define,D", value(&macros_),
        "specify a macro to define (as `macro[=[value]]`)")
      ("long_long", "enable long long support in C++ mode")
      ("variadics", "enable certain C99 extensions in C++ mode")
      ("c99", "enable C99 mode (implies `--variadics`)")
      ("c++11", "enable C++11 mode (implies `--variadics` and `--long_long`)")
      ("nostdinc++", nostdinc_help.c_str())
      ;
    // clang-format on
    return desc;
  }
}

namespace metashell
{
  data::wave_config
  parse_wave_config(bool use_templight_headers_,
                    const std::vector<std::string>& args_,
                    const std::string& metashell_binary_,
                    const boost::filesystem::path& internal_dir_,
                    metashell::iface::environment_detector& env_detector_,
                    metashell::iface::displayer& displayer_,
                    metashell::logger* logger_)
  {
    using boost::program_options::command_line_parser;

    data::wave_config result =
        use_templight_headers_ ?
            internal_clang_config(metashell_binary_, internal_dir_,
                                  env_detector_, displayer_, logger_) :
            data::wave_config();

    result.ignore_macro_redefinition = use_templight_headers_;

    std::vector<boost::filesystem::path> quote_includes;
    std::vector<boost::filesystem::path> sys_includes;

    boost::program_options::options_description desc = wave_options(
        use_templight_headers_, quote_includes, sys_includes, result.macros);

    boost::program_options::variables_map vm;
    store(command_line_parser(args_).options(desc).run(), vm);
    notify(vm);
    validate(vm);

    if (vm.count("nostdinc++"))
    {
      result.includes.quote.erase(
          std::remove_if(
              result.includes.quote.begin(), result.includes.quote.end(),
              [](const boost::filesystem::path& p_) { return p_ != "."; }),
          result.includes.quote.end());
      result.includes.sys.clear();
    }

    append(result.includes.quote, quote_includes);
    append(result.includes.sys, sys_includes);

    result.long_long = vm.count("long_long");
    result.variadics = vm.count("variadics");

    if (vm.count("c99"))
    {
      result.standard = data::wave_standard::c99;
      result.long_long = true;
    }
    else if (vm.count("c++11"))
    {
      result.standard = data::wave_standard::cpp11;
      result.long_long = true;
      result.variadics = true;
    }

    return result;
  }

  std::string wave_args(bool use_templight_headers_)
  {
    std::vector<boost::filesystem::path> quote_includes;
    std::vector<boost::filesystem::path> sys_includes;
    std::vector<std::string> macros;

    boost::program_options::options_description desc = wave_options(
        use_templight_headers_, quote_includes, sys_includes, macros);
    std::ostringstream s;
    s << "Wave options:<br />";
    const auto width = desc.get_option_column_width();
    for (const auto& option : desc.options())
    {
      if (option)
      {
        const auto arg = option->format_name();
        s << "&nbsp;&nbsp;`" << arg << "`"
          << std::string(width - arg.size() - 2, ' ') << option->description()
          << "<br />";
      }
    }
    return s.str();
  }
}
