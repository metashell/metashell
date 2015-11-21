// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/config.hpp>
#include <metashell/user_config.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/fstream_file_writer.hpp>

#include <metashell/metashell.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/join.hpp>

#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace metashell;

namespace
{
  const char* default_clang_search_path[] =
    {
      ""
      #include "default_clang_search_path.hpp"
    };

  std::string default_clang_search_path_to_string()
  {
    std::ostringstream s;
    bool first = true;
    for (auto p : default_clang_search_path)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        s << ", ";
      }
      s << p;
    }
    return s.str();
  }

  std::string directory_of_file(const std::string& path_)
  {
    boost::filesystem::path p(path_);
    p.remove_filename();
    return p.string();
  }

  std::string clang_shipped_with_metashell(
    iface::environment_detector& env_detector_
  )
  {
    return
      directory_of_file(env_detector_.path_of_executable())
      + (
        env_detector_.on_windows() ?
          "\\templight\\templight.exe" :
          "/templight_metashell"
      );
  }

  std::string detect_clang_binary(
    const std::string& user_defined_path_,
    iface::environment_detector& env_detector_,
    iface::displayer& displayer_,
    logger* logger_
  )
  {
    METASHELL_LOG(logger_, "Searching Clang binary");
    if (user_defined_path_.empty())
    {
      METASHELL_LOG(logger_, "No user override for Clang binary path.");

      const std::string clang_metashell =
        clang_shipped_with_metashell(env_detector_);

      METASHELL_LOG(
        logger_,
        "Path of Clang shipped with Metashell: " + clang_metashell
      );

      if (env_detector_.file_exists(clang_metashell))
      {
        METASHELL_LOG(
          logger_,
          "Clang shipped with Metashell is there. Choosing that."
        );
        return clang_metashell;
      }
      else
      {
        METASHELL_LOG(
          logger_,
          "Clang binary shipped with Metashell is missing. Searching for"
          " another Clang binary at the following locations: "
          + default_clang_search_path_to_string()
        );
        const std::string clang = env_detector_.search_clang_binary();

        if (clang.empty())
        {
          METASHELL_LOG(logger_, "No Clang binary found.");

          std::ostringstream s;
          s
            << "clang++ not found. Checked:" << std::endl
            << clang_metashell << std::endl;
          std::copy(
            default_clang_search_path + 1,
            default_clang_search_path
              + sizeof(default_clang_search_path) / sizeof(const char*),
            std::ostream_iterator<std::string>(s, "\n")
          );

          displayer_.show_error(s.str());
        }
        else
        {
          METASHELL_LOG(logger_, "Clang binary found: " + clang);
        }

        return clang;
      }
    }
    else
    {
      METASHELL_LOG(
        logger_,
        "User override for Clang binary: " + user_defined_path_
      );

      if (env_detector_.file_exists(user_defined_path_))
      {
        METASHELL_LOG(
          logger_,
          "User defined Clang binary exists. Choosing that."
        );
        return user_defined_path_;
      }
      else
      {
        METASHELL_LOG(logger_, "User defined Clang binary not found.");
        displayer_.show_error(
          "clang++ not found. Checked:\n" + user_defined_path_ + "\n"
        );

        return std::string();
      }
    }
  }

  bool detect_precompiled_header_usage(
    bool user_wants_precompiled_headers_,
    const config& cfg_,
    iface::displayer& displayer_,
    logger* logger_
  )
  {
    if (user_wants_precompiled_headers_)
    {
      METASHELL_LOG(logger_, "Checking if precompiled headers can be used.");
      if (cfg_.clang_path.empty())
      {
        displayer_.show_error("Disabling precompiled headers");
        METASHELL_LOG(
          logger_,
          "Disabling precompiled headers: no Clang binary is available."
        );
      }
      else
      {
        return true;
      }
    }
    else
    {
      METASHELL_LOG(logger_, "User disabled precompiled header usage.");
    }
    return false;
  }

  std::vector<std::string> clang_sysinclude(
    const std::string& clang_binary_path_,
    stdlib stdlib_,
    iface::environment_detector& env_detector_,
    logger* logger_
  )
  {
    METASHELL_LOG(logger_, "Determining Clang's sysinclude");

    if (clang_binary_path_.empty())
    {
      METASHELL_LOG(logger_, "No Clang binary is specified.");
      return env_detector_.extra_sysinclude();
    }
    else if (clang_binary_path_ == clang_shipped_with_metashell(env_detector_))
    {
      METASHELL_LOG(logger_, "Using the Clang binary shipped with Metashell.");
      return env_detector_.extra_sysinclude();
    }
    else
    {
      METASHELL_LOG(
        logger_,
        "Getting the sysinclude of the Clang binary being used."
      );
      return
        env_detector_.default_clang_sysinclude(clang_binary_path_, stdlib_);
    }
  }

  std::vector<std::string> determine_extra_clang_args(
    std::vector<std::string> extra_clang_args_,
    iface::environment_detector& env_detector_
  )
  {
    if (env_detector_.on_windows())
    {
      extra_clang_args_.push_back("-fno-ms-compatibility");
      extra_clang_args_.push_back("-U_MSC_VER");
    }
    return extra_clang_args_;
  }

  std::vector<std::string> determine_include_path(
    const std::string& clang_binary_path_,
    stdlib stdlib_,
    const std::vector<std::string>& user_include_path_,
    iface::environment_detector& env_detector_,
    logger* logger_
  )
  {
    METASHELL_LOG(
      logger_,
      "Determining include path of Clang: " + clang_binary_path_
    );

    std::vector<std::string> result =
      clang_sysinclude(clang_binary_path_, stdlib_, env_detector_, logger_);

    const std::string dir_of_executable =
      directory_of_file(env_detector_.path_of_executable());

    if (env_detector_.on_windows())
    {
      // mingw headers shipped with Metashell
      const std::string mingw_headers = dir_of_executable + "\\windows_headers";

      std::vector<std::string> wpath;
      wpath.push_back(mingw_headers);
      wpath.push_back(mingw_headers + "\\mingw32");
      if (
        clang_binary_path_.empty()
        || clang_binary_path_ == clang_shipped_with_metashell(env_detector_)
      )
      {
        wpath.push_back(dir_of_executable + "\\templight\\include");
      }

      result.insert(result.end(), wpath.begin(), wpath.end());
    }
    else
    {
      if (env_detector_.on_osx())
      {
        result.push_back(dir_of_executable + "/../include/metashell/libcxx");
      }
      result.push_back(dir_of_executable + "/../include/metashell/templight");
    }

    result.insert(
      result.end(),
      user_include_path_.begin(),
      user_include_path_.end()
    );

    METASHELL_LOG(
      logger_,
      "Include path determined: " + boost::algorithm::join(result, ";")
    );

    return result;
  }
}

config::config() :
  include_path(),
  verbose(false),
  standard_to_use(standard::cpp11),
  warnings_enabled(true),
  use_precompiled_headers(false),
  clang_path(),
  splash_enabled(true),
  stdlib_to_use(stdlib::libstdcxx)
{}

config metashell::detect_config(
  const user_config& ucfg_,
  iface::environment_detector& env_detector_,
  iface::displayer& displayer_,
  logger* logger_
)
{
  METASHELL_LOG(logger_, "Detecting config");

  config cfg;

  cfg.verbose = ucfg_.verbose;
  cfg.standard_to_use = ucfg_.standard_to_use;
  cfg.macros = ucfg_.macros;
  cfg.warnings_enabled = ucfg_.warnings_enabled;
  cfg.extra_clang_args =
    determine_extra_clang_args(ucfg_.extra_clang_args, env_detector_);

  cfg.clang_path =
    detect_clang_binary(ucfg_.clang_path, env_detector_, displayer_, logger_);

  cfg.include_path =
    determine_include_path(
      cfg.clang_path,
      ucfg_.stdlib_to_use,
      ucfg_.include_path,
      env_detector_,
      logger_
    );

  cfg.max_template_depth = ucfg_.max_template_depth;
  cfg.saving_enabled = ucfg_.saving_enabled;

  cfg.use_precompiled_headers =
    detect_precompiled_header_usage(
      ucfg_.use_precompiled_headers,
      cfg,
      displayer_,
      logger_
    );

  cfg.splash_enabled = ucfg_.splash_enabled;
  cfg.stdlib_to_use = ucfg_.stdlib_to_use;

  METASHELL_LOG(logger_, "Config detection completed");

  return cfg;
}

config metashell::empty_config(const std::string& argv0_)
{
  default_environment_detector ed(argv0_, nullptr);
  null_displayer d;
  return detect_config(user_config(), ed, d, nullptr);
}

