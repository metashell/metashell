#ifndef METASHELL_WAVE_TRACE_HPP
#define METASHELL_WAVE_TRACE_HPP

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

#include <metashell/wave_trace_impl.hpp>

#include <metashell/data/event_data_sequence.hpp>

#include <memory>

namespace metashell
{
  class wave_trace : public data::event_data_sequence<wave_trace>
  {
  public:
    wave_trace(const data::cpp_code& env_,
               const boost::optional<data::cpp_code>& exp_,
               const data::wave_config& config_);

    boost::optional<data::event_data> next();

  private:
    std::unique_ptr<wave_trace_impl> _impl;
  };
}

#endif
