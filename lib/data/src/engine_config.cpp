// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/arg_parser.hpp>
#include <metashell/data/engine_config.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/unsupported_standard_headers_allowed.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>
#include <cassert>

namespace metashell
{
  namespace data
  {
    namespace
    {
      command_line_argument_list to_clang_arguments(const engine_config& cfg_)
      {
        command_line_argument_list result = to_clang_arguments(cfg_.includes);

        for (const auto& macro : cfg_.macros)
        {
          result.push_back(clang_argument(macro));
        }

        result.push_back("-ftemplate-depth=" +
                         std::to_string(cfg_.template_instantiation_depth));

        result.push_back(command_line_argument("-std=") +
                         gcc_name(cfg_.standard));

        switch (cfg_.use_standard_headers)
        {
        case standard_headers_allowed::none:
          result.push_back("-nostdinc");
          result.push_back("-nostdinc++");
          break;
        case standard_headers_allowed::c:
          result.push_back("-nostdinc");
          break;
        case standard_headers_allowed::cpp:
          result.push_back("-nostdinc++");
          break;
        case standard_headers_allowed::all:
          break;
        }

        return result;
      }

      command_line_argument_list to_wave_arguments(const engine_config& cfg_)
      {
        command_line_argument_list result = to_wave_arguments(cfg_.includes);

        for (const auto& macro : cfg_.macros)
        {
          result.push_back(clang_argument(macro));
        }

        result.push_back(wave_name(cfg_.standard));

        switch (cfg_.use_standard_headers)
        {
        case standard_headers_allowed::none:
          result.push_back("--nostdinc++");
          break;
        case standard_headers_allowed::c:
        case standard_headers_allowed::cpp:
          throw unsupported_standard_headers_allowed(
              real_engine_name::wave, cfg_.use_standard_headers);
          break;
        case standard_headers_allowed::all:
          break;
        }

        return result;
      }

      command_line_argument_list to_arguments(real_engine_name engine_,
                                              const engine_config& cfg_)
      {
        switch (engine_)
        {
        case real_engine_name::internal:
          return to_clang_arguments(cfg_);
        case real_engine_name::pure_wave:
        case real_engine_name::wave:
          return to_wave_arguments(cfg_);
        case real_engine_name::null:
          return {};
        case real_engine_name::clang:
        case real_engine_name::gcc:
        case real_engine_name::msvc:
        case real_engine_name::templight:
          throw exception("Switching to engine " + to_string(engine_) +
                          " is not supported.");
        }
        assert(!"Invalid engine name");
        return {};
      }

      std::string
      unsupported_compiler_argument(const command_line_argument& arg_)
      {
        return "Compiler argument " + arg_ +
               " is not yet supported by Metashell.";
      }

      struct ignore_t
      {
        void operator()(const command_line_argument&) {}
        void operator()() {}
      };

      template <size_t Len>
      void append_with_suffixes(const char (&name_)[Len],
                                const std::vector<std::string>& suffixes_,
                                std::vector<std::string>& target_)
      {
        for (const std::string& suffix : suffixes_)
        {
          target_.push_back(name_ + suffix);
        }
      }

      template <size_t Len>
      void can_be_disabled(const char (&name_)[Len],
                           std::vector<std::string>& target_)
      {
        target_.push_back(name_);
        target_.push_back(std::string("no-") + name_);
      }

      std::vector<std::string> dash_g_suffixes()
      {
        const std::vector<std::string> debug_level{"", "0", "1", "2", "3"};
        const std::vector<std::string> debug_level_plus{
            "+", "0", "1", "2", "3"};

        std::vector<std::string> result{
            "en-decls",
            "split-dwarf",
            "pubnames",
            "gnu-pubnames",
            "toggle",
            "used",
            "full",
            "line-directives-only",
            "line-tables-only",
            "mlt",
            "modules",
            "lldb",
            "sce",
            "dwarf-aranges",
        };

        can_be_disabled("record-gcc-switches", result);
        can_be_disabled("strict-dwarf", result);
        can_be_disabled("column-info", result);
        can_be_disabled("embed-source", result);
        can_be_disabled("gnu-pubnames", result);
        can_be_disabled("record-command-line", result);

        append_with_suffixes("", debug_level, result);
        append_with_suffixes("gdb", debug_level, result);
        append_with_suffixes(
            "dwarf", {"", "-0", "-1", "-2", "-3", "-4", "-5"}, result);
        append_with_suffixes("stabs", debug_level_plus, result);
        append_with_suffixes("coff", debug_level, result);
        append_with_suffixes("xcoff", debug_level_plus, result);
        append_with_suffixes("vms", debug_level, result);
        append_with_suffixes("z", {"", "=none", "=zlib", "=zlib-gnu"}, result);

        return result;
      }
    }

    engine_arguments convert_to(real_engine_name engine_,
                                const engine_config& cfg_)
    {
      return {to_arguments(engine_, cfg_), engine_};
    }

    engine_config parse_vc_arguments(const command_line_argument_list& args_)
    {
      ignore_t ignore;

      engine_config result;
      result.standard = language_standard::cpp17;

      arg_parser parser(unsupported_compiler_argument);

      // clang-format off
      parser
        .with_value(
          "/I",
          "specify an additional include directory",
          result.includes.capital_i
        )
        .with_value(
          "/D",
          "specify a macro to define (as `macro[=[value]]`)",
          [&result](const command_line_argument& def_)
          { result.macros.push_back(data::macro_definition(def_)); }
        )
        .with_value(
          "/U",
          "specify a macro to undefine",
          [&result](const command_line_argument& name_)
          { result.macros.push_back(data::macro_undefinition(name_)); }
        )
        .flag(
          "/X",
          "ignore standard headers",
          [&result]
          { result.use_standard_headers = standard_headers_allowed::none; }
        )
        .with_value(
          "/EH",
          "exception handling model",
          ignore
        )
      ;
      // clang-format on

      parser.parse(args_);

      return result;
    }

    engine_config parse_clang_arguments(const command_line_argument_list& args_)
    {
      ignore_t ignore;

      engine_config result;
      result.standard = language_standard::cpp11;
      result.use_standard_headers = standard_headers_allowed::all;

      arg_parser parser(unsupported_compiler_argument);

      // clang-format off
      parser
        .with_value(
          "-I",
          "specify an additional include directory",
          result.includes.capital_i
        )
        .with_value(
          "-isystem",
          "specify an additional system include directory",
          result.includes.isystem
        )
        .with_value(
          "-iquote",
          "specify an additional quote include directory",
          result.includes.iquote
        )
        .with_value(
          "-idirafter",
          "specify an additional include directory to be checked after system headers",
          result.includes.idirafter
        )
        .with_value(
          "-D",
          "specify a macro to define (as `macro[=[value]]`)",
          [&result](const command_line_argument& def_)
          { result.macros.push_back(data::macro_definition(def_)); }
        )
        .with_value(
          "-U",
          "specify a macro to undefine",
          [&result](const command_line_argument& name_)
          { result.macros.push_back(data::macro_undefinition(name_)); }
        )
        .with_value(
          "-ftemplate-depth=",
          "specify the maximum template instantion depth",
          [&result](const command_line_argument& arg_)
          { result.template_instantiation_depth = arg_.as_int("Invalid template instantiation depth"); }
        )
        .with_value(
          "-std=",
          "specify the languge standard to use",
          [&result](const command_line_argument& arg_)
          { result.standard = parse_standard(data::real_engine_name::clang, arg_.value()); }
        )
        .with_value(
          "-W",
          "Warning-related setting",
          ignore
        )
        .flag(
          "-nostdinc",
          "ignore standard C headers",
          [&result]
          { result.use_standard_headers = disable_c(result.use_standard_headers); }
        )
        .flag(
          "-nostdinc++",
          "ignore standard C headers",
          [&result]
          { result.use_standard_headers = disable_cpp(result.use_standard_headers); }
        )
        .flag(
          "-fno-ms-compatibility",
          "disable Visual C++ compatibility",
          ignore
        )
        .flag("-g", dash_g_suffixes(), "Add debug symbols", ignore)
      ;
      // clang-format on

      parser.parse(args_);

      return result;
    }
  }
}
