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

#include <metashell/engine/auto/entry.hpp>
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
    std::map<data::engine_name, core::engine_entry> available_engines()
    {
      std::map<data::engine_name, core::engine_entry> result{
          {engine::templight::name(true), engine::templight::entry(true)},
          {engine::clang::name(), engine::clang::entry()},
          {engine::templight::name(false), engine::templight::entry(false)},
          {engine::null::name(), engine::null::entry()},
          {engine::gcc::name(), engine::gcc::entry()},
          {engine::vc::name(), engine::vc::entry()},
          {engine::wave::name_with_templight_headers(),
           engine::wave::entry_with_templight_headers()},
          {engine::wave::name(), engine::wave::entry()}};

      result.insert({engine::auto_::name(), engine::auto_::entry(result)});

      return result;
    }
  }
}
