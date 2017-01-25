#ifndef METASHELL_IFACE_ENGINE_HPP
#define METASHELL_IFACE_ENGINE_HPP

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

#include <metashell/iface/code_completer.hpp>
#include <metashell/iface/cpp_validator.hpp>
#include <metashell/iface/header_discoverer.hpp>
#include <metashell/iface/macro_discovery.hpp>
#include <metashell/iface/metaprogram_tracer.hpp>
#include <metashell/iface/preprocessor_shell.hpp>
#include <metashell/iface/type_shell.hpp>

namespace metashell
{
  namespace iface
  {
    class engine
    {
    public:
      virtual ~engine() {}

      virtual iface::type_shell& type_shell() = 0;
      virtual const iface::type_shell& type_shell() const = 0;

      virtual iface::preprocessor_shell& preprocessor_shell() = 0;
      virtual const iface::preprocessor_shell& preprocessor_shell() const = 0;

      virtual iface::code_completer& code_completer() = 0;
      virtual const iface::code_completer& code_completer() const = 0;

      virtual iface::header_discoverer& header_discoverer() = 0;
      virtual const iface::header_discoverer& header_discoverer() const = 0;

      virtual iface::metaprogram_tracer& metaprogram_tracer() = 0;
      virtual const iface::metaprogram_tracer& metaprogram_tracer() const = 0;

      virtual iface::cpp_validator& cpp_validator() = 0;
      virtual const iface::cpp_validator& cpp_validator() const = 0;

      virtual iface::macro_discovery& macro_discovery() = 0;
      virtual const iface::macro_discovery& macro_discovery() const = 0;
    };
  }
}

#endif
