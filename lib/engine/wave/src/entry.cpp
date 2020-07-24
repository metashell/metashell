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

#include <metashell/engine/clang/binary.hpp>
#include <metashell/engine/clang/header_discoverer.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

#include <metashell/data/wave_arg_parser.hpp>

#include <limits>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      namespace
      {
        std::vector<boost::filesystem::path>
        prefix_all(const std::optional<boost::filesystem::path>& prefix_,
                   std::vector<boost::filesystem::path> items_)
        {
          if (prefix_)
          {
            std::transform(items_.begin(), items_.end(), items_.begin(),
                           [&prefix_](const boost::filesystem::path& path_) {
                             return *prefix_ / path_;
                           });
          }
          return items_;
        }

        std::vector<data::feature> supported_features()
        {
          return {data::feature::preprocessor_shell(),
                  data::feature::header_discoverer(),
                  data::feature::cpp_validator(),
                  data::feature::macro_discovery(),
                  data::feature::preprocessor_tracer()};
        }

        std::vector<boost::filesystem::path> determine_clang_system_includes(
            data::standard_headers_allowed allowed_,
            const data::executable_path& metashell_binary_,
            const boost::filesystem::path& internal_dir_,
            iface::environment_detector& env_detector_,
            iface::displayer& displayer_,
            core::logger* logger_)
        {
          if (allowed_ == data::standard_headers_allowed::none)
          {
            return {};
          }

          const data::command_line_argument_list extra_clang_args;
          if (const auto clang_path = clang::find_clang_nothrow(
                  true,
                  data::engine_arguments{
                      extra_clang_args, data::real_engine_name::internal},
                  metashell_binary_, env_detector_, displayer_, logger_))
          {

            return clang::header_discoverer(
                       clang::binary(true, *clang_path, extra_clang_args,
                                     boost::filesystem::path(), internal_dir_,
                                     env_detector_, logger_))
                .include_path(data::include_type::sys, allowed_);
          }
          return {};
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
              UseTemplightHeaders, config_.engine->args, metashell_binary_,
              internal_dir_, env_detector_, displayer_, logger_);

          const std::vector<boost::filesystem::path> system_includes =
              UseTemplightHeaders ?
                  prefix_all(
                      cfg.config.includes.isysroot,
                      determine_clang_system_includes(
                          cfg.config.use_standard_headers, metashell_binary_,
                          internal_dir_, env_detector_, displayer_, logger_)) :
                  std::vector<boost::filesystem::path>();

          return make_engine(
              UseTemplightHeaders ? name_with_templight_headers() : name(),
              config_.engine->name, not_supported(),
              preprocessor_shell(cfg, system_includes), not_supported(),
              header_discoverer(cfg, system_includes), not_supported(),
              cpp_validator(cfg, system_includes),
              macro_discovery(cfg, system_includes),
              preprocessor_tracer(cfg, system_includes), supported_features(),
              [cfg] { return cfg.config; });
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
                  "<br /><br />" +
                  data::wave_arg_parser{UseTemplightHeaders}.description(
                      std::numeric_limits<int>::max())),
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
