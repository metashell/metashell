#ifndef METASHELL_DEFAULTS_PRAGMA_MAP_HPP
#define METASHELL_DEFAULTS_PRAGMA_MAP_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/command_processor_queue.hpp>
#include <metashell/core/logger.hpp>

#include <metashell/data/pragma_name.hpp>

#include <metashell/iface/pragma_handler.hpp>

#include <boost/filesystem/path.hpp>

#include <map>
#include <memory>

namespace metashell
{
  namespace defaults
  {
    std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>>
    pragma_map(core::command_processor_queue* cpq_,
               const boost::filesystem::path& mdb_temp_dir_,
               core::logger* logger_);
  }
} // namespace metashell

#endif
