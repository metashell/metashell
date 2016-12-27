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

#include <metashell/engine.hpp>
#include <metashell/engine_null.hpp>
#include <metashell/not_supported.hpp>

namespace metashell
{
  engine_entry get_engine_null_entry()
  {
    return engine_entry(
        [](const data::config& config_, const boost::filesystem::path&,
           const boost::filesystem::path&, const boost::filesystem::path&,
           iface::environment_detector&, iface::displayer&, logger*) {
          return make_engine(config_.engine, not_supported(), not_supported(),
                             not_supported(), not_supported(), not_supported(),
                             not_supported());
        },
        "",
        "An engine which does not support anything. Mainly for testing "
        "purposes.");
  }
}
