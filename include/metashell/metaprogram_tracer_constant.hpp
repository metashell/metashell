#ifndef METASHELL_METAPROGRAM_TRACER_CONSTANT_HPP
#define METASHELL_METAPROGRAM_TRACER_CONSTANT_HPP

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
  class metaprogram_tracer_constant : public iface::metaprogram_tracer
  {
  public:
    virtual data::metaprogram eval(iface::environment&,
                                   const boost::filesystem::path&,
                                   const boost::optional<std::string>&,
                                   data::metaprogram::mode_t,
                                   bool,
                                   iface::displayer&) override;
  };
}

#endif
