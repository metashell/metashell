#ifndef METASHELL_MOCK_ENVIRONMENT_DETECTOR_HPP
#define METASHELL_MOCK_ENVIRONMENT_DETECTOR_HPP

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

#include <metashell/iface/environment_detector.hpp>

#include <gmock/gmock.h>

namespace metashell
{
  namespace mock
  {
    class environment_detector : public iface::environment_detector
    {
    public:
      MOCK_METHOD0(search_clang_binary, std::optional<data::executable_path>());
      MOCK_METHOD1(file_exists, bool(const boost::filesystem::path&));

      MOCK_METHOD0(on_windows, bool());
      MOCK_METHOD0(on_osx, bool());

      MOCK_METHOD0(directory_of_executable, boost::filesystem::path());
    };
  }
}

#endif
