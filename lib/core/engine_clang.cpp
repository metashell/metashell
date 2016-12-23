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

#include <metashell/engine_clang.hpp>

#include <metashell/code_completer_clang.hpp>
#include <metashell/engine.hpp>
#include <metashell/header_discoverer_clang.hpp>
#include <metashell/preprocessor_shell_clang.hpp>
#include <metashell/template_tracer_clang.hpp>
#include <metashell/type_shell_clang.hpp>

#include <metashell/clang_binary.hpp>
#include <metashell/has_prefix.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/transformed.hpp>

using namespace metashell;

namespace
{
  const std::string default_clang_search_path[] = {""
#include "default_clang_search_path.hpp"
  };

  bool stdinc_allowed(const std::vector<std::string>& extra_clang_args_)
  {
    return find_if(extra_clang_args_.begin(), extra_clang_args_.end(),
                   [](const std::string& s_) {
                     return s_ == "-nostdinc" || s_ == "-nostdinc++";
                   }) == extra_clang_args_.end();
  }

  std::string extract_clang_binary(const std::vector<std::string>& engine_args_,
                                   iface::environment_detector& env_detector_,
                                   const std::string& metashell_path_,
                                   const std::string& engine_name_)
  {
    if (engine_args_.empty())
    {
      const std::string sample_path =
          env_detector_.on_windows() ?
              "c:\\Program Files\\LLVM\\bin\\clang++.exe" :
              "/usr/bin/clang++";
      throw std::runtime_error(
          "The engine requires that you specify the path to the clang compiler"
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
            "The path specified as the Clang binary to use (" + path +
            ") does not exist.");
      }
    }
  }

  boost::filesystem::path
  templight_shipped_with_metashell(iface::environment_detector& env_detector_)
  {
    return env_detector_.directory_of_executable() /
           (env_detector_.on_windows() ? "\\templight\\templight.exe" :
                                         "/templight_metashell");
  }

  boost::filesystem::path
  detect_clang_binary(iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      logger* logger_)
  {
    METASHELL_LOG(logger_, "Searching Clang binary");

    const boost::filesystem::path clang_metashell =
        templight_shipped_with_metashell(env_detector_);

    METASHELL_LOG(logger_, "Path of Clang shipped with Metashell: " +
                               clang_metashell.string());

    if (env_detector_.file_exists(clang_metashell))
    {
      METASHELL_LOG(
          logger_, "Clang shipped with Metashell is there. Choosing that.");
      return clang_metashell;
    }
    else
    {
      METASHELL_LOG(
          logger_,
          "Clang binary shipped with Metashell is missing. Searching for"
          " another Clang binary at the following locations: " +
              boost::algorithm::join(default_clang_search_path, ", "));
      const boost::filesystem::path clang = env_detector_.search_clang_binary();

      if (clang.empty())
      {
        using boost::adaptors::sliced;
        using boost::algorithm::join;

        METASHELL_LOG(logger_, "No Clang binary found.");

        const auto search_path_len =
            sizeof(default_clang_search_path) / sizeof(const char*);

        displayer_.show_error(
            "clang++ not found. Checked:\n" + clang_metashell.string() + "\n" +
            join(default_clang_search_path | sliced(1, search_path_len), "\n") +
            "\n");
      }
      else
      {
        METASHELL_LOG(logger_, "Clang binary found: " + clang.string());
      }

      return clang;
    }
  }

  std::string set_max_template_depth(int v_)
  {
    return "-ftemplate-depth=" + std::to_string(v_);
  }

  bool cpp_standard_set(const std::vector<std::string>& args_)
  {
    return metashell::has_prefix(args_, {"--std", "-std"});
  }

  bool max_template_depth_set(const std::vector<std::string>& args_)
  {
    return metashell::has_prefix(args_, {"-ftemplate-depth"});
  }

  std::vector<boost::filesystem::path>
  determine_include_path(const boost::filesystem::path& clang_binary_path_,
                         iface::environment_detector& env_detector_,
                         logger* logger_)
  {
    METASHELL_LOG(logger_, "Determining include path of Clang: " +
                               clang_binary_path_.string());

    std::vector<boost::filesystem::path> result;

    const boost::filesystem::path dir_of_executable =
        env_detector_.directory_of_executable();

    if (env_detector_.on_windows())
    {
      // mingw headers shipped with Metashell
      const boost::filesystem::path mingw_headers =
          dir_of_executable / "windows_headers";

      result.push_back(mingw_headers);
      result.push_back(mingw_headers / "mingw32");
      if (clang_binary_path_.empty() ||
          clang_binary_path_ == templight_shipped_with_metashell(env_detector_))
      {
        result.push_back(dir_of_executable / "templight" / "include");
      }
    }
    else
    {
      if (env_detector_.on_osx())
      {
        result.push_back(dir_of_executable / ".." / "include" / "metashell" /
                         "libcxx");
      }
      result.push_back(dir_of_executable / ".." / "include" / "metashell" /
                       "templight");
    }

    METASHELL_LOG(
        logger_, "Include path determined: " +
                     boost::algorithm::join(
                         result | boost::adaptors::transformed(
                                      [](const boost::filesystem::path& p_) {
                                        return p_.string();
                                      }),
                         ";"));

    return result;
  }

  template <bool UseInternalTemplight>
  std::vector<std::string>
  clang_args(const data::config& config_,
             const boost::filesystem::path& internal_dir_,
             iface::environment_detector& env_detector_,
             logger* logger_,
             const boost::filesystem::path& clang_path_)
  {
    std::vector<std::string> args{"-iquote", ".", "-x", "c++-header"};

    if (stdinc_allowed(config_.extra_clang_args))
    {
      args.push_back("-I");
      args.push_back(internal_dir_.string());
    }

    if (UseInternalTemplight)
    {
      args.push_back("-Wfatal-errors");

      if (env_detector_.on_windows())
      {
        args.push_back("-fno-ms-compatibility");
        args.push_back("-U_MSC_VER");
      }

      if (!cpp_standard_set(config_.extra_clang_args))
      {
        args.push_back("-std=c++0x");
      }

      if (!max_template_depth_set(config_.extra_clang_args))
      {
        args.push_back(set_max_template_depth(256));
      }

      if (stdinc_allowed(config_.extra_clang_args))
      {
        const std::vector<boost::filesystem::path> include_path =
            determine_include_path(clang_path_, env_detector_, logger_);
        args.reserve(args.size() + include_path.size());
        for (const boost::filesystem::path& p : include_path)
        {
          args.push_back("-I" + p.string());
        }
      }

      args.insert(args.end(), config_.extra_clang_args.begin(),
                  config_.extra_clang_args.end());
    }
    else if (config_.extra_clang_args.size() > 1)
    {
      args.insert(args.end(), config_.extra_clang_args.begin() + 1,
                  config_.extra_clang_args.end());
    }

    return args;
  }

  template <bool UseInternalTemplight>
  std::unique_ptr<iface::engine>
  create_clang_engine(const data::config& config_,
                      const boost::filesystem::path& internal_dir_,
                      const boost::filesystem::path& temp_dir_,
                      const boost::filesystem::path& env_filename_,
                      iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      logger* logger_)
  {
    const boost::filesystem::path clang_path =
        UseInternalTemplight ?
            detect_clang_binary(env_detector_, displayer_, logger_) :
            extract_clang_binary(config_.extra_clang_args, env_detector_,
                                 config_.metashell_binary, config_.engine);
    clang_binary cbin(clang_path, clang_args<UseInternalTemplight>(
                                      config_, internal_dir_, env_detector_,
                                      logger_, clang_path),
                      logger_);

    return make_engine(
        config_.engine,
        type_shell_clang(internal_dir_, env_filename_, cbin, logger_),
        preprocessor_shell_clang(cbin),
        code_completer_clang(
            internal_dir_, temp_dir_, env_filename_, cbin, logger_),
        header_discoverer_clang(cbin),
        template_tracer_clang(internal_dir_, env_filename_, cbin));
  }
} // anonymous namespace

engine_entry metashell::get_engine_clang_entry()
{
  return engine_entry(
      &create_clang_engine<false>,
      "<Clang binary> -std=<standard to use> [<Clang args>]",
      "Uses the Clang compiler or Templight. <Clang args> are passed to the"
      " compiler as command line-arguments. Note that Metashell requires C++11"
      " or above. If your Clang uses such a standard by default, you can omit"
      " the -std argument.");
}

engine_entry metashell::get_internal_templight_entry()
{
  return engine_entry(
      &create_clang_engine<true>, "[<Clang args>]",
      "Uses the Templight shipped with Metashell. <Clang args> are passed to"
      " the compiler as command line-arguments.");
}
