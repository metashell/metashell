#ifndef METASHELL_ENGINE_TEMPLIGHT_ENTRY_HPP
#define METASHELL_ENGINE_TEMPLIGHT_ENTRY_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/executable_path.hpp>
#include <metashell/data/real_engine_name.hpp>

#include <metashell/core/engine_entry.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  namespace engine
  {
    namespace templight
    {
      data::real_engine_name name(bool use_internal_templight_);

      core::engine_entry entry(bool use_internal_templight_,
                               data::executable_path metashell_binary_);

      bool smaller(boost::filesystem::path, boost::filesystem::path);
    } // namespace templight
  } // namespace engine
} // namespace metashell

#endif
