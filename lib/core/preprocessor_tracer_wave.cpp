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

#include <metashell/preprocessor_tracer_wave.hpp>
#include <metashell/wave_trace.hpp>

#include <metashell/filter_events.hpp>

#include <metashell/data/metaprogram.hpp>
#include <metashell/data/stdin_name.hpp>

namespace metashell
{
  preprocessor_tracer_wave::preprocessor_tracer_wave(data::wave_config config_)
    : _config(std::move(config_))
  {
  }

  data::metaprogram
  preprocessor_tracer_wave::eval(iface::environment& env_,
                                 const boost::optional<data::cpp_code>& exp_,
                                 data::metaprogram::mode_t mode_)
  {
    auto trace =
        filter_events(wave_trace(env_.get(), exp_, _config),
                      data::determine_from_line(
                          env_.get(), exp_, data::stdin_name_in_clang()),
                      mode_ != data::metaprogram::mode_t::full);

    return data::metaprogram(
        trace, mode_, exp_ ? *exp_ : data::cpp_code("<environment>"));
  }
}
