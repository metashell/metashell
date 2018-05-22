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

#include <metashell/event_data_sequence.hpp>
#include <metashell/metaprogram_tracer_constant.hpp>

#include <metashell/data/in_memory_event_data_sequence.hpp>

namespace metashell
{
  std::unique_ptr<iface::event_data_sequence>
  metaprogram_tracer_constant::eval(iface::environment&,
                                    const boost::filesystem::path&,
                                    const boost::optional<data::cpp_code>&,
                                    data::metaprogram_mode mode_,
                                    iface::displayer&)
  {
    return make_event_data_sequence_ptr(
        data::in_memory_event_data_sequence{data::cpp_code{}, mode_, {}});
  }
}
