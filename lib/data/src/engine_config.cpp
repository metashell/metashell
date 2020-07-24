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

        for (const clang_arch& arch : cfg_.archs)
        {
          result.push_back("-arch");
          result.push_back(arch.value());
        }

        if (cfg_.target)
        {
          result.push_back("-target");
          result.push_back(cfg_.target->value());
        }

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

        if (cfg_.standard_lib)
        {
          result.push_back(command_line_argument{"-stdlib="} +
                           cfg_.standard_lib->value());
        }

        if (!cfg_.warnings_enabled)
        {
          result.push_back("-w");
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
          result.push_back("-nostdinc");
          result.push_back("-nostdinc++");
          break;
        case standard_headers_allowed::c:
          result.push_back("-nostdinc++");
          break;
        case standard_headers_allowed::cpp:
          result.push_back("-nostdinc");
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
      using data::arg_number;

      ignore_t ignore;

      engine_config result;
      result.standard = language_standard::cpp17;
      result.warnings_enabled = true;

      arg_parser parser(unsupported_compiler_argument);

      // clang-format off
      parser
        .with_value(
          "/I",
          "specify an additional include directory",
          result.includes.capital_i
        )
        .with_value<arg_number::any>(
          "/D",
          "specify a macro to define (as `macro[=[value]]`)",
          [&result](const command_line_argument& def_)
          { result.macros.push_back(data::macro_definition(def_)); }
        )
        .with_value<arg_number::any>(
          "/U",
          "specify a macro to undefine",
          [&result](const command_line_argument& name_)
          { result.macros.push_back(data::macro_undefinition(name_)); }
        )
        .flag<arg_number::any>(
          "/X",
          "ignore standard headers",
          [&result]
          { result.use_standard_headers = standard_headers_allowed::none; }
        )
        .flag<arg_number::any>(
          "/w",
          "Suppresses all compiler warnings.",
          [&result] { result.warnings_enabled = false; }
        )
        .with_value<arg_number::any>(
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
      result.warnings_enabled = true;

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
        .with_value<arg_number::any>(
          "-isysroot",
          "Specify the root of the includes",
          [&result](const command_line_argument& path_)
          {
            result.includes.isysroot = path_.value();
          }
        )
        .with_value<arg_number::any>(
          "--sysroot",
          "Specify the root of the includes",
          [&result](const command_line_argument& path_)
          {
            if (!result.includes.isysroot)
            {
              result.includes.isysroot = path_.value();
            }
          }
        )
        .with_value<arg_number::any>(
          "-D",
          "specify a macro to define (as `macro[=[value]]`)",
          [&result](const command_line_argument& def_)
          { result.macros.push_back(data::macro_definition(def_)); }
        )
        .with_value<arg_number::any>(
          "-U",
          "specify a macro to undefine",
          [&result](const command_line_argument& name_)
          { result.macros.push_back(data::macro_undefinition(name_)); }
        )
        .with_value<arg_number::any>(
          "-ftemplate-depth=",
          "specify the maximum template instantion depth",
          [&result](const command_line_argument& arg_)
          { result.template_instantiation_depth = arg_.as_int("Invalid template instantiation depth"); }
        )
        .with_value<arg_number::any>(
          "-std=",
          "specify the languge standard to use",
          [&result](const command_line_argument& arg_)
          { result.standard = parse_standard(data::real_engine_name::clang, arg_.value()); }
        )
        .with_value(
          "-arch",
          "specify the architecture to use",
          result.archs
        )
        .with_value(
          "-target",
          "specify the target to use",
          result.target
        )
        .with_value(
          "-stdlib=",
          "specify the standard lib to use",
          result.standard_lib
        )
        .with_value<arg_number::any>(
          "-W",
          "Warning-related setting",
          ignore
        )
        .with_value<arg_number::any>(
          "-fvisibility",
          "Set the default ELF image symbol visibility",
          ignore
        )
        .with_value<arg_number::any>(
          "-ftls-model",
          "Alter the thread-local storage model to be used",
          ignore
        )
        .with_value<arg_number::any>(
          "-miphoneos-version-min",
          "Minimum iPhone OS version",
          ignore
        )
        .with_value<arg_number::any>(
          "-mmacosx-version-min",
          "Minimum macOS version",
          ignore
        )
        .with_value<arg_number::any>(
          "-fno-sanitize",
          "Disable sanitizers",
          ignore
        )
        .with_value<arg_number::any>(
          "-mfloat-abi",
          "Use hardware or software instructions for floating point"
          " operations and specify registers to use for floating point"
          " parameter passing and return values.",
          ignore
        )
        .with_value<arg_number::any>(
          "-march",
          "Target CPU architecture",
          ignore
        )
        .flag<arg_number::any>(
          "-nostdinc",
          "ignore standard C headers",
          [&result]
          { result.use_standard_headers = disable_c(result.use_standard_headers); }
        )
        .flag<arg_number::any>(
          "-nostdinc++",
          "ignore standard C headers",
          [&result]
          { result.use_standard_headers = disable_cpp(result.use_standard_headers); }
        )
        .flag<arg_number::any>(
          "-w",
          "Inhibit all warning messages",
          [&result] { result.warnings_enabled = false; }
        )
        .flag<arg_number::any>(
          "-fno-ms-compatibility",
          "disable Visual C++ compatibility",
          ignore
        )
        .flag<arg_number::any>(
          "-g",
          dash_g_suffixes(),
          "Add debug symbols",
          ignore
        )
        .flag<arg_number::any>(
          "-pedantic",
          "Issue all the warnings demanded by strict IDO C and ISO C++",
          ignore
        )
        .flag<arg_number::any>(
          "-pedantic-errors",
          "Give an error whenever the base standard requires a diagnostic.",
          ignore
        )
        .flag<arg_number::any>(
          "-fpic",
          "Generate position-independent code suitable for use in a shared"
          " library.",
          ignore
        )
        .flag<arg_number::any>(
          "-fpie",
          "Similar to -fpic, but generated position independent code"
          " can be only linked into executables.",
          ignore
        )
        .flag<arg_number::any>(
          "-fPIC",
          "Emit position-independent code",
          ignore
        )
        .flag<arg_number::any>(
          "-fPIE",
          "Similar to -fPIC, but generated position independent code"
          " can be only linked into executables.",
          ignore
        )
        .flag<arg_number::any>(
          "-fvisibility-inlines-hidden",
          "Declares that the user does not attempt to compare pointers to"
          " inline functions or methods where the addresses of the two"
          " functions are taken in different shared object.",
          ignore
        )
        .flag<arg_number::any>(
          "-ffunction-sections",
          "Place each function into its own section in the output file",
          ignore
        )
        .flag<arg_number::any>(
          "-fdata-sections",
          "Place each data item into its own section in the output file",
          ignore
        )
        .flag<arg_number::any>(
          "-fexceptions",
          "Enable exception hanlding",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-exceptions",
          "Disable exception hanlding",
          ignore
        )
        .flag<arg_number::any>(
          "-frtti",
          "Enable generation of information about every class"
          " with virtual functions",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-rtti",
          "Disable generation of information about every class"
          " with virtual functions",
          ignore
        )
        .flag<arg_number::any>(
          "-fbuiltin",
          "Recognize built-in functions that do not begin with"
          " __builtin_ as prefix",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-builtin",
          "Don't recognize built-in functions that do not begin with"
          " __builtin_ as prefix",
          ignore
        )
        .flag<arg_number::any>(
          "-fbuiltin-function",
          "Recognize built-in functions that do not begin with"
          " __builtin_ as prefix",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-builtin-function",
          "Don't recognize built-in functions that do not begin with"
          " __builtin_ as prefix",
          ignore
        )
        .flag<arg_number::any>(
          "-fomit-frame-pointer",
          "Don't keep the frame pointer in a register for"
          " functions that don't need one.",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-omit-frame-pointer",
          "Keep the frame pointer in a register for"
          " functions that don't need one.",
          ignore
        )
        .flag<arg_number::any>(
          "-funwind-tables",
          "Similar to -fexceptions, except that it just generates any"
          " needed static data, but does not affect the generated code in any"
          " other way.",
          ignore
        )
        .flag<arg_number::any>(
          "-fstack-protector",
          "Emit extra code to check for buffer overflows for functions that"
          " call alloca and functions with buffers larger than 8 bytes.",
          ignore
        )
        .flag<arg_number::any>(
          "-fstack-protector-all",
          "Like -fstack-protector except that all functions are protected.",
          ignore
        )
        .flag<arg_number::any>(
          "-fstack-protector-strong",
          "Like -fstack-protector but includes additional functions to be"
          " protected.",
          ignore
        )
        .flag<arg_number::any>(
          "-fstack-protector-explicit",
          "Like -fstack-protector but only protects those functions which"
          " have the stack_protect attribute",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-stack-protector",
          "Disable the stack protector check",
          ignore
        )
        .flag<arg_number::any>(
          "-flto",
          "Enable link-time optimisation",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-lto",
          "Disable link-time optimisation",
          ignore
        )
        .flag<arg_number::any>(
          "-O0",
          "Reduce compilation time and make debugging"
          " produce the expected result",
          ignore
        )
        .flag<arg_number::any>("-O", "Optimize", ignore)
        .flag<arg_number::any>("-O1", "Optimize", ignore)
        .flag<arg_number::any>("-O2", "Optimize even more", ignore)
        .flag<arg_number::any>("-O3", "Optimize yet more", ignore)
        .flag<arg_number::any>("-Os", "Optimize for size", ignore)
        .flag<arg_number::any>("-Oz", "Optimize for size further", ignore)
        .flag<arg_number::any>(
          "-Ofast",
          "Disregard strict standards compliance",
          ignore
        )
        .flag<arg_number::any>("-Og", "Optimize debugging experience", ignore)
        .flag<arg_number::any>("-m32", "Generate code for 32-bit ABI", ignore)
        .flag<arg_number::any>("-m64", "Generate code for 64-bit ABI", ignore)
        .flag<arg_number::any>(
          "-ffreestanding",
          "Assert that compilation targets a freestanding environment.",
          ignore
        )
        .flag<arg_number::any>(
          "-fcommon",
          "Place uninitialized global variables in a common block.",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-common",
          "Place uninitialized global variables in the data section of the"
          " object file.",
          ignore
        )
        .flag<arg_number::any>(
          "-fstrict-aliasing",
          "Allow the compiler to assume the strictest aliasing rules"
          " applicable to the language being compiled.",
          ignore
        )
        .flag<arg_number::any>(
          "-fno-strict-aliasing",
          "Don't allow the compiler to assume the strictest aliasing rules"
          " applicable to the language being compiled.",
          ignore
        )
        .flag<arg_number::any>(
          "-pthread",
          "Define additional macros required for using the POSIX threads"
          " library.",
          ignore
        )
        .flag<arg_number::any>(
          "-static",
          "Prevent linking with the static libraries.",
          ignore
        )
        .flag<arg_number::any>(
          "-mkernel",
          "Enable kernel development mode.",
          ignore
        )
        .flag<arg_number::any>(
          "-fobjc-arc",
          "Synthesize retain and release calls for Objective-C pointers",
          ignore
        )
      ;
      // clang-format on

      for (std::string instr :
           {"mmx",      "sse",      "sse2",     "sse3",       "ssse3",
            "sse4",     "sse4a",    "sse4.1",   "sse4.2",     "avx",
            "avx2",     "avx512f",  "avx512pf", "avx512er",   "avx512cd",
            "avx512vl", "avx512bw", "avx512dq", "avx512ifma", "avx512vbmi",
            "sha",      "aes",      "pclmul",   "clflushopt", "fsgsbase",
            "rdrnd",    "f16c",     "fma",      "fma4",       "prefetchwt1",
            "xop",      "lwp",      "3dnow",    "3dnowa",     "popcnt",
            "abm",      "bmi",      "bmi2",     "lzcnt",      "fxsr",
            "xsave",    "xsaveopt", "xsavec",   "xsaves",     "rtm",
            "tbm",      "mpx",      "mwaitx",   "clzero",     "pku"})
      {
        parser.flag(data::command_line_argument("-m" + instr),
                    "Enable " + instr + " instructions", arg_number::any,
                    ignore);
        parser.flag(data::command_line_argument("-mno-" + instr),
                    "Disable " + instr + " instructions", arg_number::any,
                    ignore);
      }

      parser.parse(args_);

      return result;
    }
  }
}
