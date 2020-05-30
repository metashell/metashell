#ifndef METASHELL_IFACE_METAPROGRAM_TRACER_HPP
#define METASHELL_IFACE_METAPROGRAM_TRACER_HPP

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

#include <metashell/data/feature.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/result.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/environment.hpp>
#include <metashell/iface/event_data_sequence.hpp>

#include <boost/filesystem/path.hpp>

#include <memory>
#include <optional>
#include <string>

namespace metashell
{
  namespace iface
  {
    class metaprogram_tracer
    {
    public:
      virtual ~metaprogram_tracer() {}

      virtual std::unique_ptr<event_data_sequence>
      eval(iface::environment& env_,
           const boost::filesystem::path& temp_dir_,
           const std::optional<data::cpp_code>& expression_,
           data::metaprogram_mode mode_,
           iface::displayer& displayer_) = 0;

      static data::feature name_of_feature()
      {
        return data::feature::metaprogram_tracer();
      }
    };
  }
}

#endif
