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

#include <metashell/engine_wave.hpp>

#include <metashell/cpp_validator_wave.hpp>
#include <metashell/engine.hpp>
#include <metashell/header_discoverer_wave.hpp>
#include <metashell/macro_discovery_wave.hpp>
#include <metashell/not_supported.hpp>
#include <metashell/parse_wave_config.hpp>
#include <metashell/preprocessor_shell_wave.hpp>
#include <metashell/preprocessor_tracer_wave.hpp>

using namespace metashell;

namespace
{
  std::vector<data::feature> supported_features()
  {
    return {data::feature::preprocessor_shell(),
            data::feature::header_discoverer(), data::feature::cpp_validator(),
            data::feature::macro_discovery(),
            data::feature::preprocessor_tracer()};
  }

  template <bool UseTemplightHeaders>
  std::unique_ptr<iface::engine>
  create_wave_engine(const data::config& config_,
                     const boost::filesystem::path& internal_dir_,
                     const boost::filesystem::path&,
                     const boost::filesystem::path&,
                     iface::environment_detector& env_detector_,
                     iface::displayer& displayer_,
                     logger* logger_)
  {
    const data::wave_config cfg = parse_wave_config(
        UseTemplightHeaders, config_.extra_clang_args, config_.metashell_binary,
        internal_dir_, env_detector_, displayer_, logger_);
    return make_engine(config_.engine, not_supported(),
                       preprocessor_shell_wave(cfg), not_supported(),
                       header_discoverer_wave(cfg), not_supported(),
                       cpp_validator_wave(cfg), macro_discovery_wave(cfg),
                       preprocessor_tracer_wave(cfg), supported_features());
  }

  template <bool UseTemplightHeaders>
  engine_entry get_engine_entry()
  {
    return engine_entry(
        &create_wave_engine<UseTemplightHeaders>, "<Wave options>",
        data::markdown_string(
            "Uses [Boost.Wave](http://boost.org/libs/wave), which is a "
            "preprocessor. Only the preprocessor shell is supported." +
            (UseTemplightHeaders ?
                 " It uses the headers of Templight deployed with Metashell." :
                 std::string()) +
            "<br /><br />" + wave_args(UseTemplightHeaders)),
        supported_features());
  }
} // anonymous namespace

engine_entry metashell::get_engine_wave_entry()
{
  return get_engine_entry<false>();
}

engine_entry metashell::get_engine_wave_entry_with_templight_headers()
{
  return get_engine_entry<true>();
}
