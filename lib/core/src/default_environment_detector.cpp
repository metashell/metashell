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

#include <metashell/core/default_environment_detector.hpp>
#include <metashell/core/header_file_environment.hpp>

#include <metashell/data/default_clang_search_path.hpp>

#include <just/environment.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#ifdef __APPLE__
#include <errno.h>
#include <libproc.h>
#include <mach-o/dyld.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef __FreeBSD__
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

namespace metashell
{
  namespace core
  {
    namespace
    {
      bool file_exists_impl(const boost::filesystem::path& path_)
      {
        std::ifstream f(path_.c_str());
        return !(f.fail() || f.bad());
      }

#if !(defined _WIN32 || defined __FreeBSD__ || defined __APPLE__)
      boost::filesystem::path read_link(const boost::filesystem::path& path_)
      {
        const int step = 64;
        std::vector<char> buff;
        ssize_t res;
        do
        {
          buff.resize(buff.size() + step);
          res = readlink(path_.c_str(), buff.data(), buff.size());
        } while (res == ssize_t(buff.size()));
        return res == -1 ? path_ :
                           std::string(buff.begin(), buff.begin() + res);
      }
#endif

#if defined __OpenBSD__
      boost::filesystem::path current_working_directory()
      {
        std::vector<char> buff(1);
        while (getcwd(buff.data(), buff.size()) == NULL)
        {
          buff.resize(buff.size() * 2);
        }
        return buff.data();
      }
#endif

      boost::filesystem::path path_of_executable_impl(const std::string& argv0_)
      {
#ifdef _WIN32
        (void)argv0_;

        char path[MAX_PATH];
        GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
        return path;
#elif defined __FreeBSD__
        (void)argv0_;

        int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
        std::vector<char> buff(1);
        size_t cb = buff.size();
        while (sysctl(mib, 4, buff.data(), &cb, NULL, 0) != 0)
        {
          buff.resize(buff.size() * 2);
          cb = buff.size();
        }
        return std::string(buff.begin(), buff.begin() + cb);
#elif defined __OpenBSD__
        if (argv0_.empty() || argv0_[0] == '/')
        {
          return argv0_;
        }
        else if (argv0_.find('/') != std::string::npos)
        {
          // This code assumes that the application never changes the working
          // directory
          return current_working_directory() / argv0_;
        }
        else
        {
          const std::string p = just::environment::get("PATH");
          std::vector<boost::filesystem::path> path;
          boost::split(path, p, [](char c_) { return c_ == ':'; });
          for (const auto& s : path)
          {
            const boost::filesystem::path fn = s / argv0_;
            if (file_exists(fn))
            {
              return fn;
            }
          }
          return "";
        }
#elif defined __APPLE__
        (void)argv0_;
        uint32_t size = 0;
        // ask for size first
        if (_NSGetExecutablePath(nullptr, &size) == 0)
        {
          return "";
        }
        std::vector<char> buff(size + 1);
        if (_NSGetExecutablePath(buff.data(), &size) == 0)
        {
          return std::string(buff.data());
        }
        return "";
#else
        (void)argv0_;
        return read_link("/proc/self/exe");
#endif
      }
    } // anonymous namespace

    boost::filesystem::path default_environment_detector::path_of_executable()
    {
      // resolve symlinks with boost::filesystem::canonical
      return canonical(path_of_executable_impl(_argv0));
    }

    default_environment_detector::default_environment_detector(
        const std::string& argv0_)
      : _argv0(argv0_)
    {
    }

    std::optional<data::executable_path>
    default_environment_detector::search_clang_binary()
    {
      const data::default_clang_search_path sp;
      const auto p = std::find_if(sp.begin(), sp.end(), file_exists_impl);
      return p == sp.end() ? std::nullopt :
                             std::make_optional(data::executable_path(*p));
    }

    bool default_environment_detector::file_exists(
        const boost::filesystem::path& path_)
    {
      return file_exists_impl(path_);
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

    boost::filesystem::path
    default_environment_detector::directory_of_executable()
    {
      boost::filesystem::path p = path_of_executable();
      p.remove_filename();
      return p;
    }
  }
}
