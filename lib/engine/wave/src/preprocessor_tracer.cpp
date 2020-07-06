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

#include <metashell/engine/wave/preprocessor_tracer.hpp>
#include <metashell/engine/wave/trace.hpp>

#include <metashell/core/filter_events.hpp>

#include <metashell/data/stdin_name.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      preprocessor_tracer::preprocessor_tracer(
          data::wave_config config_,
          std::vector<boost::filesystem::path> system_includes_)
        : _config(std::move(config_)),
          _system_includes(std::move(system_includes_))
      {
      }

      std::unique_ptr<iface::event_data_sequence>
      preprocessor_tracer::eval(iface::environment& env_,
                                const std::optional<data::cpp_code>& exp_,
                                data::metaprogram_mode mode_)
      {
        return core::filter_events(
            trace(env_.get(), exp_, _config, mode_, _system_includes),
            data::determine_from_line(
                env_.get(), exp_, data::stdin_name_in_clang()));
      }
    }
  }
}
