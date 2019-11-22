// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/vc/entry.hpp>

#include <metashell/engine/vc/binary.hpp>
#include <metashell/engine/vc/cpp_validator.hpp>
#include <metashell/engine/vc/header_discoverer.hpp>
#include <metashell/engine/vc/preprocessor_shell.hpp>

#include <metashell/process/exception.hpp>
#include <metashell/process/run.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

#include <metashell/data/exception.hpp>

#include <just/environment.hpp>

#include <regex>

namespace metashell
{
  namespace engine
  {
    namespace vc
    {
      namespace
      {
        std::vector<data::feature> supported_features()
        {
          return {data::feature::preprocessor_shell(),
                  data::feature::header_discoverer(),
                  data::feature::cpp_validator()};
        }

        bool this_engine(const data::command_line_argument_list& args_)
        {
          if (const auto first = args_.front())
          {
            const data::executable_path exe(*first);
            try
            {
              const std::string dash_v =
                  process::run(data::command_line(exe, {"-v"}), "")
                      .standard_error;
              return regex_search(dash_v, std::regex("^Microsoft ")) &&
                     regex_search(dash_v, std::regex("[\\n\\r]cl : .*-v"));
            }
            catch (const process::exception&)
            {
              return false;
            }
          }
          else
          {
            return false;
          }
        }

        data::executable_path
        extract_vc_binary(const data::engine_arguments& engine_,
                          iface::environment_detector& env_detector_,
                          const data::executable_path& metashell_path_)
        {
          if (const auto first = engine_.args.front())
          {
            const data::executable_path exe(*first);
            if (env_detector_.file_exists(exe))
            {
              return exe;
            }
            else
            {
              throw data::exception(
                  "The path specified as the vc binary to use (" +
                  to_string(exe) + ") does not exist.");
            }
          }
          else
          {
            throw data::exception(
                "The engine requires that you specify the path to cl.exe after "
                "--. For example: " +
                metashell_path_ + " --engine " + engine_.name +
                " -- C:\\Program Files (x86)\\Microsoft Visual Studio "
                "14.0\\VC\\bin\\cl.exe\"");
          }
        }

        data::command_line_argument_list
        vc_args(const data::command_line_argument_list& extra_vc_args_,
                const boost::filesystem::path& internal_dir_)
        {
          data::command_line_argument_list args{"/I" + internal_dir_.string()};

          if (extra_vc_args_.size() > 1)
          {
            args.append(extra_vc_args_.begin() + 1, extra_vc_args_.end());
          }

          return args;
        }

        std::unique_ptr<iface::engine>
        create_vc_engine(const data::shell_config& config_,
                         const data::executable_path& metashell_binary_,
                         const boost::filesystem::path& internal_dir_,
                         const boost::filesystem::path& temp_dir_,
                         const boost::filesystem::path& env_filename_,
                         iface::environment_detector& env_detector_,
                         core::logger* logger_)
        {
          using core::not_supported;

          if (!just::environment::exists("INCLUDE"))
          {
            throw data::exception(
                "To use the Visual C++ engine, please run Metashell "
                " from the Visual Studio Developer Prompt.");
          }

          binary cbin(extract_vc_binary(
                          *config_.engine, env_detector_, metashell_binary_),
                      vc_args(config_.engine->args, internal_dir_), temp_dir_,
                      logger_);

          return make_engine(
              name(), config_.engine->name, not_supported(),
              preprocessor_shell(cbin), not_supported(),
              header_discoverer(cbin), not_supported(),
              cpp_validator(internal_dir_, env_filename_, cbin, logger_),
              not_supported(), not_supported(), supported_features());
        }
      } // anonymous namespace

      data::real_engine_name name() { return data::real_engine_name::msvc; }

      core::engine_entry entry(data::executable_path metashell_binary_)
      {
        return core::engine_entry(
            [metashell_binary_](const data::shell_config& config_,
                                const boost::filesystem::path& internal_dir_,
                                const boost::filesystem::path& temp_dir_,
                                const boost::filesystem::path& env_filename_,
                                iface::environment_detector& env_detector_,
                                iface::displayer&, core::logger* logger_) {
              return create_vc_engine(config_, metashell_binary_, internal_dir_,
                                      temp_dir_, env_filename_, env_detector_,
                                      logger_);
            },
            "<path to cl.exe> [<cl.exe args>]",
            data::markdown_string(
                "Uses the [Visual C++ "
                "compiler](https://www.visualstudio.com/vs/cplusplus). "
                "`<cl.exe args>` are passed to the compiler as command "
                "line-arguments. Note that currently only the preprocessor "
                "shell is supported. You need to run Metashell from the Visual "
                "Studio Developer Prompt to use this engine."),
            supported_features(), this_engine);
      }
    }
  }
}
