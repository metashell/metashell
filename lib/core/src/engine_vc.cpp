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

#include <metashell/core/cpp_validator_vc.hpp>
#include <metashell/core/engine.hpp>
#include <metashell/core/engine_vc.hpp>
#include <metashell/core/header_discoverer_vc.hpp>
#include <metashell/core/not_supported.hpp>
#include <metashell/core/preprocessor_shell_vc.hpp>
#include <metashell/core/vc_binary.hpp>

#include <metashell/data/exception.hpp>

#include <just/environment.hpp>

namespace metashell
{
  namespace core
  {
    namespace
    {
      std::vector<data::feature> supported_features()
      {
        return {data::feature::preprocessor_shell(),
                data::feature::header_discoverer(),
                data::feature::cpp_validator()};
      }

      std::string
      extract_vc_binary(const std::vector<std::string>& engine_args_,
                        iface::environment_detector& env_detector_,
                        const std::string& metashell_path_,
                        const std::string& engine_name_)
      {
        if (engine_args_.empty())
        {
          const std::string sample_path =
              "\"C:\\Program Files (x86)\\Microsoft Visual Studio "
              "14.0\\VC\\bin\\cl.exe\"";
          throw data::exception(
              "The engine requires that you specify the path to cl.exe"
              " after --. For example: " +
              metashell_path_ + " --engine " + engine_name_ + " -- " +
              sample_path);
        }
        else
        {
          const std::string path = engine_args_.front();
          if (env_detector_.file_exists(path))
          {
            return path;
          }
          else
          {
            throw data::exception(
                "The path specified as the vc binary to use (" + path +
                ") does not exist.");
          }
        }
      }

      std::vector<std::string>
      vc_args(const std::vector<std::string>& extra_vc_args_,
              const boost::filesystem::path& internal_dir_)
      {
        std::vector<std::string> args{"/I" + internal_dir_.string()};

        if (extra_vc_args_.size() > 1)
        {
          args.insert(
              args.end(), extra_vc_args_.begin() + 1, extra_vc_args_.end());
        }

        return args;
      }

      std::unique_ptr<iface::engine>
      create_vc_engine(const data::config& config_,
                       const boost::filesystem::path& internal_dir_,
                       const boost::filesystem::path& temp_dir_,
                       const boost::filesystem::path& env_filename_,
                       iface::environment_detector& env_detector_,
                       iface::displayer&,
                       logger* logger_)
      {
        if (!just::environment::exists("INCLUDE"))
        {
          throw data::exception(
              "To use the Visual C++ engine, please run Metashell "
              " from the Visual Studio Developer Prompt.");
        }

        const boost::filesystem::path vc_path = extract_vc_binary(
            config_.active_shell_config().engine_args, env_detector_,
            config_.metashell_binary, config_.active_shell_config().engine);
        vc_binary cbin(
            vc_path,
            vc_args(config_.active_shell_config().engine_args, internal_dir_),
            temp_dir_, logger_);

        return make_engine(
            config_.active_shell_config().engine, not_supported(),
            preprocessor_shell_vc(cbin), not_supported(),
            header_discoverer_vc(cbin), not_supported(),
            cpp_validator_vc(internal_dir_, env_filename_, cbin, logger_),
            not_supported(), not_supported(), supported_features());
      }
    } // anonymous namespace

    engine_entry get_engine_vc_entry()
    {
      return engine_entry(
          &create_vc_engine, "<path to cl.exe> [<cl.exe args>]",
          data::markdown_string(
              "Uses the [Visual C++ "
              "compiler](https://www.visualstudio.com/vs/cplusplus). `<cl.exe "
              "args>` are passed to the compiler as command line-arguments. "
              "Note "
              "that currently only the preprocessor shell is supported. You "
              "need "
              "to run Metashell from the Visual Studio Developer Prompt to use "
              "this engine."),
          supported_features());
    }
  }
}
