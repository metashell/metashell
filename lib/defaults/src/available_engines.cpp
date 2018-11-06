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

#include <metashell/defaults/available_engines.hpp>

#include <metashell/engine/null/entry.hpp>
#include <metashell/engine/vc/entry.hpp>

#include <metashell/core/engine_clang.hpp>
#include <metashell/core/engine_entry.hpp>
#include <metashell/core/engine_gcc.hpp>
#include <metashell/core/engine_templight.hpp>
#include <metashell/core/engine_wave.hpp>

namespace metashell
{
  namespace defaults
  {
    std::map<std::string, core::engine_entry> available_engines()
    {
      return {{"internal", core::get_internal_templight_entry()},
              {"clang", core::get_engine_clang_entry()},
              {"templight", core::get_engine_templight_entry()},
              {"null", engine::null::entry()},
              {"gcc", core::get_engine_gcc_entry()},
              {"msvc", engine::vc::entry()},
              {"wave", core::get_engine_wave_entry_with_templight_headers()},
              {"pure_wave", core::get_engine_wave_entry()}};
    }
  }
}
