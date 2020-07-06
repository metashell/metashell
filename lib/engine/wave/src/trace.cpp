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

#include <metashell/engine/wave/trace.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      trace::trace(const data::cpp_code& env_,
                   const std::optional<data::cpp_code>& exp_,
                   const data::wave_config& config_,
                   data::metaprogram_mode mode_,
                   const std::vector<boost::filesystem::path>& system_includes_)
        : _impl(new trace_impl(env_, exp_, config_, system_includes_)),
          _root_name(exp_ ? *exp_ : data::cpp_code("<environment>")),
          _mode(mode_)
      {
      }

      std::optional<data::event_data> trace::next() { return _impl->next(); }

      const data::cpp_code& trace::root_name() const { return _root_name; }

      data::metaprogram_mode trace::mode() const { return _mode; }
    }
  }
}
