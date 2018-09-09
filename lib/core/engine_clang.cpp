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

#include <metashell/core/engine_clang.hpp>
#include <metashell/core/code_completer_clang.hpp>
#include <metashell/core/cpp_validator_clang.hpp>
#include <metashell/core/engine.hpp>
#include <metashell/core/header_discoverer_clang.hpp>
#include <metashell/core/macro_discovery_clang.hpp>
#include <metashell/core/metaprogram_tracer_clang.hpp>
#include <metashell/core/not_supported.hpp>
#include <metashell/core/preprocessor_shell_clang.hpp>
#include <metashell/core/type_shell_clang.hpp>
#include <metashell/core/clang_binary.hpp>

using namespace metashell;

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

  std::unique_ptr<iface::engine>
  create_clang_engine(const data::config& config_,
                      const boost::filesystem::path& internal_dir_,
                      const boost::filesystem::path& temp_dir_,
                      const boost::filesystem::path& env_filename_,
                      iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      logger* logger_)
  {
    const clang_binary cbin(
        false, find_clang(false, config_.active_shell_config().engine_args,
                          config_.metashell_binary,
                          config_.active_shell_config().engine, env_detector_,
                          displayer_, logger_),
        config_.active_shell_config().engine_args, internal_dir_, env_detector_,
        logger_);

    return make_engine(
        config_.active_shell_config().engine,
        type_shell_clang(internal_dir_, env_filename_, cbin, logger_),
        preprocessor_shell_clang(cbin),
        code_completer_clang(
            internal_dir_, temp_dir_, env_filename_, cbin, logger_),
        header_discoverer_clang(cbin), metaprogram_tracer_clang(cbin),
        cpp_validator_clang(internal_dir_, env_filename_, cbin, logger_),
        macro_discovery_clang(cbin), not_supported(), supported_features());
  }
} // anonymous namespace

engine_entry metashell::get_engine_clang_entry()
{
  return engine_entry(
      &create_clang_engine,
      "<Clang binary> -std=<standard to use> [<Clang args>]",
      data::markdown_string(
          "Uses the [Clang compiler](http://clang.llvm.org). `<Clang args>` "
          "are passed to the compiler as command line-arguments. Note that "
          "Metashell requires C++11 or above. If your Clang uses such a "
          "standard by default, you can omit the `-std` argument. Metaprogram "
          "debugging (MDB) is supported only when Clang has been patched with "
          "[templight](https://github.com/mikael-s-persson/templight)"),
      supported_features());
}
