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

#include <metashell/metashell.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/header_file_environment.hpp>

#include <just/environment.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#  include <errno.h>
#  include <libproc.h>
#endif

#ifndef _WIN32
#  include <unistd.h>
#endif

#ifdef __FreeBSD__
#  include <sys/types.h>
#  include <sys/sysctl.h>
#endif

using namespace metashell;

namespace
{
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

#if !(defined _WIN32 || defined __FreeBSD__ || defined __APPLE__)
  std::string read_link(const std::string& path_)
  {
    const int step = 64;
    std::vector<char> buff;
    ssize_t res;
    do
    {
      buff.resize(buff.size() + step);
      res = readlink(path_.c_str(), buff.data(), buff.size());
    }
    while (res == ssize_t(buff.size()));
    return res == -1 ? path_ : std::string(buff.begin(), buff.begin() + res);
  }
#endif

#ifdef __OpenBSD__
  std::string current_working_directory()
  {
    std::vector<char> buff(1);
    while (getcwd(buff.data(), buff.size()) == NULL)
    {
      buff.resize(buff.size() * 2);
    }
    return buff.data();
  }
#endif
}

default_environment_detector::default_environment_detector(
  const std::string& argv0_
) :
  _argv0(argv0_)
{}

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

bool default_environment_detector::on_osx()
{
#ifdef __APPLE__
  return true;
#else
  return false;
#endif
}

std::string default_environment_detector::path_of_executable()
{
#ifdef _WIN32
  char path[MAX_PATH];
  GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
  return path;
#elif defined __FreeBSD__
  int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
  std::vector<char> buff(1);
  size_t cb = buff.size();
  while (sysctl(mib, 4, buff.data(), &cb, NULL, 0) != 0)
  {
    buff.resize(buff.size() * 2);
    cb = buff.size();
  }
  return std::string(buff.begin(), buff.begin() + cb);
#elif defined __OpenBSD__
  if (_argv0.empty() || _argv0[0] == '/')
  {
    return _argv0;
  }
  else if (_argv0.find('/') != std::string::npos)
  {
    // This code assumes that the application never changes the working
    // directory
    return current_working_directory() + "/" + _argv0;
  }
  else
  {
    const std::string p = just::environment::get("PATH");
    std::vector<std::string> path;
    boost::split(path, p, [](char c_) { return c_ == ';'; });
    for (const auto& s : path)
    {
      const std::string fn = s + "/" + _argv0;
      if (file_exists(fn))
      {
        return fn;
      }
    }
    return "";
  }
#elif defined __APPLE__
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
  const pid_t pid = getpid();
  if (proc_pidpath(pid, pathbuf, sizeof(pathbuf)) <= 0) {
    std::cerr << "PID " << pid << ": proc_pidpath ();\n"
	  << "   " << strerror(errno) << '\n';
  }
  return pathbuf;
#else
  return read_link("/proc/self/exe");
#endif
}

