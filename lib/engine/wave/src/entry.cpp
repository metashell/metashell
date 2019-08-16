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

#include <metashell/engine/wave/entry.hpp>

#include <metashell/engine/wave/cpp_validator.hpp>
#include <metashell/engine/wave/header_discoverer.hpp>
#include <metashell/engine/wave/macro_discovery.hpp>
#include <metashell/engine/wave/parse_config.hpp>
#include <metashell/engine/wave/preprocessor_shell.hpp>
#include <metashell/engine/wave/preprocessor_tracer.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      namespace
      {
        std::vector<data::feature> supported_features()
        {
          return {data::feature::preprocessor_shell(),
                  data::feature::header_discoverer(),
                  data::feature::cpp_validator(),
                  data::feature::macro_discovery(),
                  data::feature::preprocessor_tracer()};
        }

        template <bool UseTemplightHeaders>
        std::unique_ptr<iface::engine>
        create_wave_engine(const data::shell_config& config_,
                           const data::executable_path& metashell_binary_,
                           const boost::filesystem::path& internal_dir_,
                           iface::environment_detector& env_detector_,
                           iface::displayer& displayer_,
                           core::logger* logger_)
        {
          using core::not_supported;

          const data::wave_config cfg = parse_config(
              UseTemplightHeaders, config_.engine_args, metashell_binary_,
              internal_dir_, env_detector_, displayer_, logger_);
          return make_engine(
              UseTemplightHeaders ? name_with_templight_headers() : name(),
              config_.engine, not_supported(), preprocessor_shell(cfg),
              not_supported(), header_discoverer(cfg), not_supported(),
              cpp_validator(cfg), macro_discovery(cfg),
              preprocessor_tracer(cfg), supported_features());
        }

        template <bool UseTemplightHeaders>
        core::engine_entry
        get_engine_entry(data::executable_path metashell_binary_)
        {
          return core::engine_entry(
              [metashell_binary_](const data::shell_config& config_,
                                  const boost::filesystem::path& internal_dir_,
                                  const boost::filesystem::path&,
                                  const boost::filesystem::path&,
                                  iface::environment_detector& env_detector_,
                                  iface::displayer& displayer_,
                                  core::logger* logger_) {
                return create_wave_engine<UseTemplightHeaders>(
                    config_, metashell_binary_, internal_dir_, env_detector_,
                    displayer_, logger_);
              },
              "<Wave options>",
              data::markdown_string(
                  "Uses [Boost.Wave](http://boost.org/libs/wave), which is a "
                  "preprocessor. Only the preprocessor shell is supported." +
                  (UseTemplightHeaders ? " It uses the headers of Templight "
                                         "deployed with Metashell." :
                                         std::string()) +
                  "<br /><br />" + args(UseTemplightHeaders)),
              supported_features(), core::never_used_by_auto());
        }
      } // anonymous namespace

      data::real_engine_name name()
      {
        return data::real_engine_name::pure_wave;
      }

      core::engine_entry entry(data::executable_path metashell_binary_)
      {
        return get_engine_entry<false>(std::move(metashell_binary_));
      }

      data::real_engine_name name_with_templight_headers()
      {
        return data::real_engine_name::wave;
      }

      core::engine_entry
      entry_with_templight_headers(data::executable_path metashell_binary_)
      {
        return get_engine_entry<true>(std::move(metashell_binary_));
      }
    }
  }
}
