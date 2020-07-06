#ifndef METASHELL_ENGINE_CONSTANT_METAPROGRAM_TRACER_HPP
#define METASHELL_ENGINE_CONSTANT_METAPROGRAM_TRACER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/metaprogram_tracer.hpp>

namespace metashell
{
  namespace engine
  {
    namespace constant
    {
      class metaprogram_tracer : public iface::metaprogram_tracer
      {
      public:
        virtual std::unique_ptr<iface::event_data_sequence>
        eval(iface::environment&,
             const boost::filesystem::path&,
             const std::optional<data::cpp_code>&,
             data::metaprogram_mode,
             iface::displayer&) override;
      };
    }
  }
}

#endif
