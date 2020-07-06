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

#include <metashell/data/executable_path.hpp>

#include <boost/filesystem/path.hpp>

#include <optional>

namespace metashell
{
  namespace iface
  {
    class environment_detector
    {
    public:
      virtual ~environment_detector() {}

      virtual std::optional<data::executable_path> search_clang_binary() = 0;
      virtual bool file_exists(const boost::filesystem::path& path_) = 0;

      bool file_exists(const data::executable_path& exe_)
      {
        return file_exists(exe_.value());
      }

      // Available as a runtime function for cross-platform unit testing
      virtual bool on_windows() = 0;
      virtual bool on_osx() = 0;

      virtual boost::filesystem::path directory_of_executable() = 0;
    };
  }
}

#endif
