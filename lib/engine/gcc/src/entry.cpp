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

#include <metashell/engine/gcc/entry.hpp>

#include <metashell/engine/clang/binary.hpp>

#include <metashell/engine/clang/cpp_validator.hpp>
#include <metashell/engine/clang/header_discoverer.hpp>
#include <metashell/engine/clang/macro_discovery.hpp>
#include <metashell/engine/clang/preprocessor_shell.hpp>

#include <metashell/process/exception.hpp>
#include <metashell/process/run.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

#include <regex>

namespace metashell
{
  namespace engine
  {
    namespace gcc
    {
      namespace
      {
        std::vector<data::feature> supported_features()
        {
          return {data::feature::preprocessor_shell(),
                  data::feature::header_discoverer(),
                  data::feature::cpp_validator(),
                  data::feature::macro_discovery()};
        }

        bool this_engine(const data::command_line_argument_list& args_)
        {
          if (const auto first = args_.front())
          {
            const data::executable_path exe(*first);

            try
            {
              return regex_search(
                  process::run(data::command_line(exe, {"-v"}), "")
                      .standard_error,
                  std::regex("[\\n\\r]gcc version "));
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

        bool
        stdinc_allowed(const data::command_line_argument_list& extra_gcc_args_)
        {
          return find_if(extra_gcc_args_.begin(), extra_gcc_args_.end(),
                         [](const data::command_line_argument& s_) {
                           return s_ == "-nostdinc" || s_ == "-nostdinc++";
                         }) == extra_gcc_args_.end();
        }

        data::executable_path
        extract_gcc_binary(const data::command_line_argument_list& engine_args_,
                           iface::environment_detector& env_detector_,
                           const data::executable_path& metashell_path_,
                           const data::engine_name& engine_)
        {
          if (const auto first = engine_args_.front())
          {
            const data::executable_path path(*first);
            if (env_detector_.file_exists(path))
            {
              return path;
            }
            else
            {
              throw std::runtime_error(
                  "The path specified as the gcc binary to use (" +
                  to_string(path) + ") does not exist.");
            }
          }
          else
          {
            throw std::runtime_error(
                "The engine requires that you specify the path to the gcc "
                "compiler after --. For example: " +
                metashell_path_ + " --engine " + engine_ +
                " -- /usr/bin/g++ -std=c++11");
          }
        }

        data::command_line_argument_list
        gcc_args(const data::command_line_argument_list& extra_gcc_args_,
                 const boost::filesystem::path& internal_dir_)
        {
          data::command_line_argument_list args{"-iquote", ".", "-x", "c++"};

          if (stdinc_allowed(extra_gcc_args_))
          {
            args.push_back("-I", internal_dir_.string());
          }

          if (extra_gcc_args_.size() > 1)
          {
            args.append(extra_gcc_args_.begin() + 1, extra_gcc_args_.end());
          }

          return args;
        }

        std::unique_ptr<iface::engine>
        create_gcc_engine(const data::shell_config& config_,
                          const data::executable_path& metashell_binary_,
                          const boost::filesystem::path& internal_dir_,
                          const boost::filesystem::path&,
                          const boost::filesystem::path& env_filename_,
                          iface::environment_detector& env_detector_,
                          iface::displayer&,
                          core::logger* logger_)
        {
          using core::not_supported;

          clang::binary cbin(
              extract_gcc_binary(config_.engine_args, env_detector_,
                                 metashell_binary_, config_.engine),
              gcc_args(config_.engine_args, internal_dir_), logger_);

          return core::make_engine(
              name(), config_.engine, not_supported(),
              clang::preprocessor_shell(cbin), not_supported(),
              clang::header_discoverer(cbin), not_supported(),
              clang::cpp_validator(internal_dir_, env_filename_, cbin, logger_),
              clang::macro_discovery(cbin), not_supported(),
              supported_features());
        }
      } // anonymous namespace

      data::real_engine_name name() { return data::real_engine_name::gcc; }

      core::engine_entry entry(data::executable_path metashell_binary_)
      {
        return core::engine_entry(
            [metashell_binary_](const data::shell_config& config_,
                                const boost::filesystem::path& internal_dir_,
                                const boost::filesystem::path& temp_dir_,
                                const boost::filesystem::path& env_filename_,
                                iface::environment_detector& env_detector_,
                                iface::displayer& displayer_,
                                core::logger* logger_) {
              return create_gcc_engine(config_, metashell_binary_,
                                       internal_dir_, temp_dir_, env_filename_,
                                       env_detector_, displayer_, logger_);
            },
            "<gcc binary> -std=<standard to use> [<gcc args>]",
            data::markdown_string(
                "Uses the [gcc compiler](https://gcc.gnu.org). `<gcc args>` "
                "are passed to the compiler as command line-arguments. Note "
                "that Metashell requires C++11 or above. If your gcc uses such "
                "a standard by default, you can omit the `-std` argument. Also "
                "note that currently only the preprocessor shell is "
                "supported."),
            supported_features(), this_engine);
      }
    }
  }
}
