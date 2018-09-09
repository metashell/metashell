#ifndef MDB_COMMAND_HANDLER_MAP_HPP
#define MDB_COMMAND_HANDLER_MAP_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/core/mdb_command.hpp>

#include <boost/container/flat_map.hpp>
#include <boost/optional.hpp>

#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace metashell
{

  class mdb_command_handler_map
  {
  public:
    typedef std::vector<mdb_command> commands_t;

    mdb_command_handler_map(const commands_t& commands);

    // <command, args>
    boost::optional<std::tuple<mdb_command, std::string>>
    get_command_for_line(const std::string& line) const;

    const commands_t& get_commands() const;

  private:
#ifdef _WIN32
    // Fails to compile on Windows (Visual C++ 2013, Boost 1.55)
    typedef std::map<std::string, std::size_t> key_command_map_t;
#else
    typedef boost::container::flat_map<std::string, std::size_t>
        key_command_map_t;
#endif

    commands_t commands;

    // This map's key indexes into commands
    key_command_map_t key_command_map;
  };
}

#endif
