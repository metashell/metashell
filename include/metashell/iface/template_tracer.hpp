#ifndef METASHELL_IFACE_TEMPLATE_TRACER_HPP
#define METASHELL_IFACE_TEMPLATE_TRACER_HPP

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

#include <metashell/data/metaprogram.hpp>
#include <metashell/data/result.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/environment.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>

namespace metashell
{
  namespace iface
  {
    class template_tracer
    {
    public:
      virtual ~template_tracer() {}

      virtual data::metaprogram
      eval(iface::environment& env_,
           const boost::filesystem::path& temp_dir_,
           const boost::optional<std::string>& expression_,
           data::metaprogram::mode_t mode_,
           bool use_precompiled_headers_,
           iface::displayer& displayer_) = 0;

      static std::string name_of_feature() { return "template_tracer"; }
    };
  }
}

#endif
