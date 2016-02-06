#ifndef METASHELL_ENGINE_FACTORY_HPP
#define METASHELL_ENGINE_FACTORY_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/engine.hpp>
#include <metashell/iface/environment_detector.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/data/config.hpp>
#include <metashell/logger.hpp>

#include <boost/filesystem/path.hpp>

#include <memory>
#include <functional>

namespace metashell
{
  typedef std::function<std::unique_ptr<iface::engine>(
      const data::config&,
      const boost::filesystem::path&,
      const boost::filesystem::path&,
      iface::environment_detector&,
      iface::displayer&,
      logger*)> engine_factory;

  template <class Engine>
  engine_factory factory_for()
  {
    return [](const data::config& config_,
              const boost::filesystem::path& internal_dir_,
              const boost::filesystem::path& env_filename_,
              iface::environment_detector& env_detector_,
              iface::displayer& displayer_, logger* logger_)
    {
      return std::unique_ptr<iface::engine>(
          new Engine(config_, internal_dir_, env_filename_, env_detector_,
                     displayer_, logger_));
    };
  }
}

#endif
