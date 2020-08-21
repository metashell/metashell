#ifndef METASHELL_DEFAULT_ENVIRONMENT_DETECTOR_HPP
#define METASHELL_DEFAULT_ENVIRONMENT_DETECTOR_HPP

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

namespace metashell
{
  namespace core
  {
    class default_environment_detector : public iface::environment_detector
    {
    public:
      explicit default_environment_detector(const std::string& argv0_);

      virtual std::optional<data::executable_path>
      search_clang_binary() override;
      virtual bool file_exists(const boost::filesystem::path& path_) override;

      virtual bool on_windows() override;
      virtual bool on_osx() override;

      virtual boost::filesystem::path directory_of_executable() override;

    private:
      boost::filesystem::path path_of_executable();

      std::string _argv0;
    };
  }
}

#endif
