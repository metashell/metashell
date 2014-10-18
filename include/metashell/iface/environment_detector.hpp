#ifndef METASHELL_IFACE_ENVIRONMENT_DETECTOR_HPP
#define METASHELL_IFACE_ENVIRONMENT_DETECTOR_HPP

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

#include <string>
#include <vector>

namespace metashell
{
  class config;

  namespace iface
  {
    class environment_detector
    {
    public:
      virtual ~environment_detector() {}

      virtual std::string search_clang_binary() = 0;
      virtual bool file_exists(const std::string& path_) = 0;

      // Available as a runtime function for cross-platform unit testing
      virtual bool on_windows() = 0;
      virtual bool on_osx() = 0;

      virtual void append_to_path(const std::string& path_) = 0;

      virtual std::vector<std::string> default_clang_sysinclude(
        const std::string& clang_path_
      ) = 0;

      virtual std::vector<std::string> extra_sysinclude() = 0;

      virtual std::string path_of_executable() = 0;

      virtual bool clang_binary_works_with_libclang(const config& cfg_) = 0;
    };
  }
}

#endif

