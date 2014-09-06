// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include "cxindex.hpp"

#include <metashell/metashell.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/clang_binary.hpp>
#include <metashell/header_file_environment.hpp>

#include <just/environment.hpp>

#include <algorithm>
#include <stdexcept>
#include <fstream>

using namespace metashell;

namespace
{
  const char* extra_sysinclude[] =
    {
      ""
      #include "extra_sysinclude.hpp"
    };

  const char* default_clang_search_path[] =
    {
      ""
      #include "default_clang_search_path.hpp"
    };

  bool file_exists(const std::string& path_)
  {
    std::ifstream f(path_.c_str());
    return !(f.fail() || f.bad());
  }

  template <class It>
  std::string search_clang(It begin_, It end_)
  {
    const It p = std::find_if(begin_, end_, file_exists);
    return p == end_ ? "" : *p;
  }

  std::string default_clang_path()
  {
    return
      search_clang(
        default_clang_search_path + 1,
        default_clang_search_path
          + sizeof(default_clang_search_path) / sizeof(const char*)
      );
  }
}

std::string default_environment_detector::search_clang_binary()
{
  return default_clang_path();
}

bool default_environment_detector::file_exists(const std::string& path_)
{
  return ::file_exists(path_);
}

bool default_environment_detector::on_windows() 
{
#ifdef _WIN32
  return true;
#else
  return false;
#endif
}

void default_environment_detector::append_to_path(const std::string& path_)
{
  just::environment::append_to_path(path_);
}

std::vector<std::string> default_environment_detector::default_clang_sysinclude(
  const std::string& clang_path_
)
{
  return default_sysinclude(clang_binary(clang_path_));
}

std::string default_environment_detector::path_of_executable()
{
#ifdef _WIN32
  char path[MAX_PATH];
  GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
  return path;
#else
  throw std::runtime_error("path_of_executable is not implemented");
#endif
}

std::vector<std::string> default_environment_detector::extra_sysinclude()
{
  return
    std::vector<std::string>(
      ::extra_sysinclude + 1,
      ::extra_sysinclude
        + sizeof(::extra_sysinclude) / sizeof(const char*)
    );
}

bool default_environment_detector::clang_binary_works_with_libclang(
  const config& cfg_
)
{
  config cfg(cfg_);
  cfg.use_precompiled_headers = true;

  const unsaved_file src("<stdin>", "typedef foo bar;");

  try
  {
    header_file_environment env(cfg);
    env.append("struct foo {};");

    cxindex index;
    std::unique_ptr<cxtranslationunit> tu = index.parse_code(src, env);
    return tu->errors_begin() == tu->errors_end();
  }
  catch (...)
  {
    return false;
  }
}

