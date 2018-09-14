#ifndef METASHELL_PARSE_WAVE_CONFIG_HPP
#define METASHELL_PARSE_WAVE_CONFIG_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/wave_config.hpp>

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/environment_detector.hpp>

#include <metashell/core/logger.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace core
  {
    data::wave_config
    parse_wave_config(bool use_templight_headers_,
                      const std::vector<std::string>& args_,
                      const std::string& metashell_binary_,
                      const boost::filesystem::path& internal_dir_,
                      iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      logger* logger_);

    std::string wave_args(bool use_templight_headers_);
  }
}

#endif
