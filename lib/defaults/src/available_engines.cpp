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

#include <metashell/engine/clang/entry.hpp>
#include <metashell/engine/gcc/entry.hpp>
#include <metashell/engine/null/entry.hpp>
#include <metashell/engine/templight/entry.hpp>
#include <metashell/engine/vc/entry.hpp>
#include <metashell/engine/wave/entry.hpp>

#include <metashell/core/engine_entry.hpp>

namespace metashell
{
  namespace defaults
  {
    std::map<data::real_engine_name, core::engine_entry>
    available_engines(const data::executable_path& metashell_binary_)
    {
      std::map<data::real_engine_name, core::engine_entry> result{
          {engine::templight::name(true),
           engine::templight::entry(true, metashell_binary_)},
          {engine::clang::name(), engine::clang::entry(metashell_binary_)},
          {engine::templight::name(false),
           engine::templight::entry(false, metashell_binary_)},
          {engine::null::name(), engine::null::entry()},
          {engine::gcc::name(), engine::gcc::entry(metashell_binary_)},
          {engine::vc::name(), engine::vc::entry(metashell_binary_)},
          {engine::wave::name_with_templight_headers(),
           engine::wave::entry_with_templight_headers(metashell_binary_)},
          {engine::wave::name(), engine::wave::entry(metashell_binary_)}};

      return result;
    }
  } // namespace defaults
} // namespace metashell
