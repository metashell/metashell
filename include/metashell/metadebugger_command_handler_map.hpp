#ifndef METADEBUGGER_COMMAND_HANDLER_MAP_HPP
#define METADEBUGGER_COMMAND_HANDLER_MAP_HPP

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

#include <vector>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include <metashell/metadebugger_command.hpp>

namespace metashell {

class metadebugger_command_handler_map {
public:
  typedef std::vector<metadebugger_command> command_map_t;

  metadebugger_command_handler_map(const command_map_t& command_map);

  // <command, args>
  boost::optional<std::tuple<metadebugger_command, std::string>>
    get_command_for_line(const std::string& line) const;

private:
  command_map_t command_map;
};

}

#endif

