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
#include <metashell/data/config.hpp>
#include <metashell/logger.hpp>

#include <memory>
#include <functional>

namespace metashell
{
  typedef
    std::function<
      std::unique_ptr<iface::engine>(
        const data::config&,
        const std::string&,
        const std::string&,
        iface::environment_detector&,
        logger*
      )
    >
    engine_factory;
}

#endif

