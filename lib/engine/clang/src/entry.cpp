// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/clang/binary.hpp>
#include <metashell/engine/clang/code_completer.hpp>
#include <metashell/engine/clang/cpp_validator.hpp>
#include <metashell/engine/clang/entry.hpp>
#include <metashell/engine/clang/header_discoverer.hpp>
#include <metashell/engine/clang/macro_discovery.hpp>
#include <metashell/engine/clang/metaprogram_tracer.hpp>
#include <metashell/engine/clang/preprocessor_shell.hpp>
#include <metashell/engine/clang/this_engine.hpp>
#include <metashell/engine/clang/type_shell.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        std::vector<data::feature> supported_features()
        {
          return {data::feature::type_shell(),
                  data::feature::preprocessor_shell(),
                  data::feature::code_completer(),
                  data::feature::header_discoverer(),
                  data::feature::metaprogram_tracer(),
                  data::feature::cpp_validator(),
                  data::feature::macro_discovery()};
        }

        bool this_engine(const data::command_line_argument_list& args_)
        {
          if (const auto front = args_.front())
          {
            const data::executable_path exe(*front);
            return is_clang(exe) && !is_templight(exe);
          }
          else
          {
            return false;
          }
        }

        std::unique_ptr<iface::engine>
        create_clang_engine(const data::shell_config& config_,
                            const data::executable_path& metashell_binary_,
                            const boost::filesystem::path& internal_dir_,
                            const boost::filesystem::path& temp_dir_,
                            const boost::filesystem::path& env_filename_,
                            iface::environment_detector& env_detector_,
                            iface::displayer& displayer_,
                            core::logger* logger_)
        {
          using core::not_supported;

          const binary cbin(
              false,
              find_clang(false, config_.engine_args, metashell_binary_,
                         config_.engine, env_detector_, displayer_, logger_),
              config_.engine_args, internal_dir_, env_detector_, logger_);

          return core::make_engine(
              name(), config_.engine,
              type_shell(internal_dir_, env_filename_, cbin, logger_),
              preprocessor_shell(cbin),
              code_completer(
                  internal_dir_, temp_dir_, env_filename_, cbin, logger_),
              header_discoverer(cbin), metaprogram_tracer(cbin),
              cpp_validator(internal_dir_, env_filename_, cbin, logger_),
              macro_discovery(cbin), not_supported(), supported_features());
        }
      } // anonymous namespace

      data::engine_name name() { return data::engine_name::clang; }

      core::engine_entry entry(data::executable_path metashell_binary_)
      {
        return core::engine_entry(
            [metashell_binary_](
                const data::shell_config& config_,
                const boost::filesystem::path& internal_dir_,
                const boost::filesystem::path& temp_dir_,
                const boost::filesystem::path& env_filename_,
                const std::map<data::engine_name, core::engine_entry>&,
                iface::environment_detector& env_detector_,
                iface::displayer& displayer_, core::logger* logger_) {
              return create_clang_engine(
                  config_, metashell_binary_, internal_dir_, temp_dir_,
                  env_filename_, env_detector_, displayer_, logger_);
            },
            "<Clang binary> -std=<standard to use> [<Clang args>]",
            data::markdown_string(
                "Uses the [Clang compiler](http://clang.llvm.org). `<Clang "
                "args>` are passed to the compiler as command line-arguments. "
                "Note that Metashell requires C++11 or above. If your Clang "
                "uses such a standard by default, you can omit the `-std` "
                "argument. Metaprogram debugging (MDB) is supported only when "
                "Clang has been patched with "
                "[templight](https://github.com/mikael-s-persson/templight)"),
            supported_features(), this_engine);
      }
    }
  }
}
