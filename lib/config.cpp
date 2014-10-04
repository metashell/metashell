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

#include <metashell/metashell.hpp>

#include <boost/filesystem/path.hpp>

#include <fstream>
#include <algorithm>
#include <iostream>
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
      + (env_detector_.on_windows() ? "\\clang\\clang.exe" : "/clang_metashell");
  }

  std::string detect_clang_binary(
    const std::string& user_defined_path_,
    iface::environment_detector& env_detector_,
    std::ostream& stderr_
  )
  {
    if (user_defined_path_.empty())
    {
      const std::string clang_metashell =
        clang_shipped_with_metashell(env_detector_);

      if (env_detector_.file_exists(clang_metashell))
      {
        return clang_metashell;
      }
      else
      {
        const std::string clang = env_detector_.search_clang_binary();

        if (clang.empty())
        {
          stderr_
            << "Error: clang++ not found. Checked:" << std::endl
            << clang_metashell << std::endl;
          std::copy(
            default_clang_search_path + 1,
            default_clang_search_path
              + sizeof(default_clang_search_path) / sizeof(const char*),
            std::ostream_iterator<std::string>(stderr_, "\n")
          );
        }

        return clang;
      }
    }
    else if (env_detector_.file_exists(user_defined_path_))
    {
      return user_defined_path_;
    }
    else
    {
      stderr_
        << "Error: clang++ not found. Checked:" << std::endl
        << user_defined_path_ << std::endl;

      return std::string();
    }
  }

  bool detect_precompiled_header_usage(
    bool user_wants_precompiled_headers_,
    iface::environment_detector& env_detector_,
    const config& cfg_,
    std::ostream& stderr_
  )
  {
    if (user_wants_precompiled_headers_)
    {
      if (!cfg_.clang_path.empty())
      {
        return env_detector_.clang_binary_works_with_libclang(cfg_);
      }
      else
      {
        stderr_ << "Disabling precompiled headers" << std::endl;
      }
    }
    return false;
  }

  std::vector<std::string> clang_sysinclude(
    const std::string& clang_binary_path_,
    iface::environment_detector& env_detector_
  )
  {
    return
      (
        clang_binary_path_.empty() ||
        clang_binary_path_ == clang_shipped_with_metashell(env_detector_)
      ) ?
        env_detector_.extra_sysinclude() :
        env_detector_.default_clang_sysinclude(clang_binary_path_);
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
    const std::vector<std::string>& user_include_path_,
    iface::environment_detector& env_detector_
  )
  {
    std::vector<std::string> result =
      clang_sysinclude(clang_binary_path_, env_detector_);

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
        wpath.push_back(dir_of_executable + "\\clang\\include");
      }

      result.insert(result.end(), wpath.begin(), wpath.end());
    }
    else
    {
      result.push_back(dir_of_executable + "/../include/metashell/clang");
    }

    result.insert(
      result.end(),
      user_include_path_.begin(),
      user_include_path_.end()
    );

    return result;
  }
}

config::config() :
  include_path(),
  verbose(false),
  syntax_highlight(true),
  indent(true),
  standard_to_use(standard::cpp11),
  warnings_enabled(true),
  use_precompiled_headers(false),
  clang_path()
{}

config metashell::detect_config(
  const user_config& ucfg_,
  iface::environment_detector& env_detector_,
  std::ostream& stderr_
)
{
  config cfg;

  cfg.verbose = ucfg_.verbose;
  cfg.syntax_highlight = ucfg_.syntax_highlight;
  cfg.indent = ucfg_.indent;
  cfg.standard_to_use = ucfg_.standard_to_use;
  cfg.macros = ucfg_.macros;
  cfg.warnings_enabled = ucfg_.warnings_enabled;
  cfg.extra_clang_args =
    determine_extra_clang_args(ucfg_.extra_clang_args, env_detector_);

  cfg.clang_path =
    detect_clang_binary(ucfg_.clang_path, env_detector_, stderr_);

  cfg.include_path =
    determine_include_path(cfg.clang_path, ucfg_.include_path, env_detector_);

  cfg.max_template_depth = ucfg_.max_template_depth;
  cfg.saving_enabled = ucfg_.saving_enabled;

  if (env_detector_.on_windows())
  {
    // To find libclang.dll
    if (
      cfg.clang_path.empty()
      || cfg.clang_path == clang_shipped_with_metashell(env_detector_)
    )
    {
      env_detector_.append_to_path(
        directory_of_file(env_detector_.path_of_executable()) + "\\clang"
      );
    }
    else
    {
      env_detector_.append_to_path(directory_of_file(cfg.clang_path));
    }
  }

  cfg.use_precompiled_headers =
    detect_precompiled_header_usage(
      ucfg_.use_precompiled_headers,
      env_detector_,
      cfg,
      stderr_
    );

  return cfg;
}

config metashell::empty_config(const std::string& argv0_)
{
  default_environment_detector ed(argv0_);
  std::ostringstream s;
  return detect_config(user_config(), ed, s);
}

