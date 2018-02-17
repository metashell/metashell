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

#include <metashell/engine_gcc.hpp>

#include <metashell/cpp_validator_clang.hpp>
#include <metashell/engine.hpp>
#include <metashell/header_discoverer_clang.hpp>
#include <metashell/macro_discovery_clang.hpp>
#include <metashell/not_supported.hpp>
#include <metashell/preprocessor_shell_clang.hpp>

#include <metashell/clang_binary.hpp>

using namespace metashell;

namespace
{
  std::vector<data::feature> supported_features()
  {
    return {data::feature::preprocessor_shell(),
            data::feature::header_discoverer(), data::feature::cpp_validator(),
            data::feature::macro_discovery()};
  }

  bool stdinc_allowed(const std::vector<std::string>& extra_gcc_args_)
  {
    return find_if(extra_gcc_args_.begin(), extra_gcc_args_.end(),
                   [](const std::string& s_) {
                     return s_ == "-nostdinc" || s_ == "-nostdinc++";
                   }) == extra_gcc_args_.end();
  }

  std::string extract_gcc_binary(const std::vector<std::string>& engine_args_,
                                 iface::environment_detector& env_detector_,
                                 const std::string& metashell_path_,
                                 const std::string& engine_name_)
  {
    if (engine_args_.empty())
    {
      const std::string sample_path = "/usr/bin/g++";
      throw std::runtime_error(
          "The engine requires that you specify the path to the gcc compiler"
          " after --. For example: " +
          metashell_path_ + " --engine " + engine_name_ + " -- " + sample_path +
          " -std=c++11");
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
        throw std::runtime_error(
            "The path specified as the gcc binary to use (" + path +
            ") does not exist.");
      }
    }
  }

  std::vector<std::string>
  gcc_args(const std::vector<std::string>& extra_gcc_args_,
           const boost::filesystem::path& internal_dir_)
  {
    std::vector<std::string> args{"-iquote", ".", "-x", "c++"};

    if (stdinc_allowed(extra_gcc_args_))
    {
      args.push_back("-I");
      args.push_back(internal_dir_.string());
    }

    if (extra_gcc_args_.size() > 1)
    {
      args.insert(
          args.end(), extra_gcc_args_.begin() + 1, extra_gcc_args_.end());
    }

    return args;
  }

  std::unique_ptr<iface::engine>
  create_gcc_engine(const data::config& config_,
                    const boost::filesystem::path& internal_dir_,
                    const boost::filesystem::path&,
                    const boost::filesystem::path& env_filename_,
                    iface::environment_detector& env_detector_,
                    iface::displayer&,
                    logger* logger_)
  {
    const boost::filesystem::path clang_path = extract_gcc_binary(
        config_.active_shell_config().engine_args, env_detector_,
        config_.metashell_binary, config_.active_shell_config().engine);
    clang_binary cbin(
        clang_path,
        gcc_args(config_.active_shell_config().engine_args, internal_dir_),
        logger_);

    return make_engine(
        config_.active_shell_config().engine, not_supported(),
        preprocessor_shell_clang(cbin), not_supported(),
        header_discoverer_clang(cbin), not_supported(),
        cpp_validator_clang(internal_dir_, env_filename_, cbin, logger_),
        macro_discovery_clang(cbin), not_supported(), supported_features());
  }
} // anonymous namespace

engine_entry metashell::get_engine_gcc_entry()
{
  return engine_entry(
      &create_gcc_engine, "<gcc binary> -std=<standard to use> [<gcc args>]",
      data::markdown_string(
          "Uses the [gcc compiler](https://gcc.gnu.org). `<gcc args>` are "
          "passed to the compiler as command line-arguments. Note that "
          "Metashell requires C++11 or above. If your gcc uses such a standard "
          "by default, you can omit the `-std` argument. Also note that "
          "currently only the preprocessor shell is supported."),
      supported_features());
}
