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

#include <metashell/engine/null/entry.hpp>

#include <metashell/core/engine.hpp>
#include <metashell/core/not_supported.hpp>

namespace metashell
{
  namespace engine
  {
    namespace null
    {
      namespace
      {
        std::vector<data::feature> supported_features() { return {}; }
      }

      data::engine_name name() { return data::engine_name("null"); }

      core::engine_entry entry()
      {
        using core::not_supported;

        return core::engine_entry(
            [](const data::config& config_, const boost::filesystem::path&,
               const boost::filesystem::path&, const boost::filesystem::path&,
               const std::map<data::engine_name, core::engine_entry>&,
               iface::environment_detector&, iface::displayer&, core::logger*) {
              return core::make_engine(
                  name(), config_.active_shell_config().engine, not_supported(),
                  not_supported(), not_supported(), not_supported(),
                  not_supported(), not_supported(), not_supported(),
                  not_supported(), supported_features());
            },
            "",
            data::markdown_string("An engine which does not support anything. "
                                  "Mainly for testing purposes."),
            supported_features(), core::never_used_by_auto());
      }
    }
  }
}
