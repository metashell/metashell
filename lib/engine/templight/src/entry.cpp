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

#include <metashell/engine/templight/entry.hpp>
#include <metashell/engine/templight/metaprogram_tracer.hpp>

#include <metashell/engine/clang/binary.hpp>
#include <metashell/engine/clang/code_completer.hpp>
#include <metashell/engine/clang/cpp_validator.hpp>
#include <metashell/engine/clang/header_discoverer.hpp>
#include <metashell/engine/clang/macro_discovery.hpp>
#include <metashell/engine/clang/preprocessor_shell.hpp>
#include <metashell/engine/clang/type_shell.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

namespace metashell
{
  namespace engine
  {
    namespace templight
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

        template <bool UseInternalTemplight>
        std::unique_ptr<iface::engine>
        create_templight_engine(const data::config& config_,
                                const boost::filesystem::path& internal_dir_,
                                const boost::filesystem::path& temp_dir_,
                                const boost::filesystem::path& env_filename_,
                                iface::environment_detector& env_detector_,
                                iface::displayer& displayer_,
                                core::logger* logger_)
        {
          using core::not_supported;

          const clang::binary cbin(
              UseInternalTemplight,
              clang::find_clang(UseInternalTemplight,
                                config_.active_shell_config().engine_args,
                                config_.metashell_binary,
                                config_.active_shell_config().engine,
                                env_detector_, displayer_, logger_),
              config_.active_shell_config().engine_args, internal_dir_,
              env_detector_, logger_);

          return core::make_engine(
              config_.active_shell_config().engine,
              clang::type_shell(internal_dir_, env_filename_, cbin, logger_),
              clang::preprocessor_shell(cbin),
              clang::code_completer(
                  internal_dir_, temp_dir_, env_filename_, cbin, logger_),
              clang::header_discoverer(cbin), metaprogram_tracer(cbin),
              clang::cpp_validator(internal_dir_, env_filename_, cbin, logger_),
              clang::macro_discovery(cbin), not_supported(),
              supported_features());
        }
      } // anonymous namespace

      core::engine_entry entry(bool use_internal_templight_)
      {
        return use_internal_templight_ ?
                   core::engine_entry(
                       &create_templight_engine<true>, "[<Clang args>]",
                       data::markdown_string(
                           "Uses the "
                           "[Templight](https://github.com/mikael-s-persson/"
                           "templight) "
                           "shipped with Metashell. `<Clang args>` are passed "
                           "to"
                           " the compiler as command line-arguments."),
                       supported_features()) :
                   core::engine_entry(&create_templight_engine<false>,
                                      "<Templight binary> -std=<standard to "
                                      "use> [<Clang args>]",
                                      data::markdown_string(
                                          "Uses "
                                          "[Templight](https://github.com/"
                                          "mikael-s-persson/templight). "
                                          "`<Clang args>` are passed to the "
                                          "compiler as command "
                                          "line-arguments. Note that Metashell "
                                          "requires C++11 or above. If "
                                          "your Templight uses such a standard "
                                          "by default, you can omit "
                                          "the "
                                          "`-std` argument."),
                                      supported_features());
      }
    }
  }
}
