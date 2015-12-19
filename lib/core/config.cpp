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

  std::string clang_shipped_with_metashell(
    iface::environment_detector& env_detector_
  )
  {
    return
      env_detector_.directory_of_executable()
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
}

data::config metashell::detect_config(
  const data::user_config& ucfg_,
  iface::environment_detector& env_detector_,
  iface::displayer& displayer_,
  logger* logger_
)
{
  METASHELL_LOG(logger_, "Detecting config");

  data::config cfg;

  cfg.verbose = ucfg_.verbose;
  cfg.extra_clang_args = ucfg_.extra_clang_args;

  cfg.clang_path =
    detect_clang_binary(ucfg_.clang_path, env_detector_, displayer_, logger_);

  cfg.saving_enabled = ucfg_.saving_enabled;

  cfg.use_precompiled_headers = ucfg_.use_precompiled_headers;

  cfg.splash_enabled = ucfg_.splash_enabled;

  METASHELL_LOG(logger_, "Config detection completed");

  return cfg;
}

data::config metashell::empty_config(const std::string& argv0_)
{
  default_environment_detector ed(argv0_);
  null_displayer d;
  return detect_config(data::user_config(), ed, d, nullptr);
}

