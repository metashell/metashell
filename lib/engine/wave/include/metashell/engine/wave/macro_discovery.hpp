#ifndef METASHELL_ENGINE_WAVE_MACRO_DISCOVERY_HPP
#define METASHELL_ENGINE_WAVE_MACRO_DISCOVERY_HPP

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

#include <metashell/iface/macro_discovery.hpp>

#include <metashell/data/wave_config.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      class macro_discovery : public iface::macro_discovery
      {
      public:
        explicit macro_discovery(data::wave_config);

        virtual data::cpp_code macros(const iface::environment& env_) override;

      private:
        data::wave_config _config;
      };
    }
  }
}

#endif
