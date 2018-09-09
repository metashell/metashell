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

#include <metashell/core/available_engines.hpp>
#include <metashell/core/engine_clang.hpp>
#include <metashell/core/engine_entry.hpp>
#include <metashell/core/engine_gcc.hpp>
#include <metashell/core/engine_null.hpp>
#include <metashell/core/engine_templight.hpp>
#include <metashell/core/engine_vc.hpp>
#include <metashell/core/engine_wave.hpp>

namespace metashell
{
  std::map<std::string, engine_entry> available_engines()
  {
    return {{"internal", metashell::get_internal_templight_entry()},
            {"clang", metashell::get_engine_clang_entry()},
            {"templight", metashell::get_engine_templight_entry()},
            {"null", metashell::get_engine_null_entry()},
            {"gcc", metashell::get_engine_gcc_entry()},
            {"msvc", metashell::get_engine_vc_entry()},
            {"wave", metashell::get_engine_wave_entry_with_templight_headers()},
            {"pure_wave", metashell::get_engine_wave_entry()}};
  }
}
