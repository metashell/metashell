
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

#include <metashell/metadebugger_command_handler_map.hpp>
#include <metashell/metadebugger_shell.hpp>

#include <sstream>

#include <boost/range/iterator_range_core.hpp>
#include <boost/algorithm/string/predicate.hpp>


namespace metashell {

metadebugger_command_handler_map::metadebugger_command_handler_map(
    const command_map_t& command_map_arg) :
  command_map(command_map_arg)
{
  std::sort(command_map.begin(), command_map.end());
}

boost::optional<std::tuple<metadebugger_command, std::string>>
metadebugger_command_handler_map::get_command_for_line(
    const std::string& line) const
{

  std::stringstream line_stream(line);
  std::string command;

  if (!(line_stream >> command)) {
    return boost::none;
  }

  command_map_t::const_iterator lower =
    std::lower_bound(command_map.begin(), command_map.end(), command);

  using boost::algorithm::starts_with;

  if (lower == command_map.end() ||
      !starts_with(lower->get_key(), command))
  {
    return boost::none;
  }

  // Check if the found command is unambiguous
  if (std::next(lower) != command_map.end() &&
      starts_with(std::next(lower)->get_key(), command))
  {
    return boost::none;
  }

  line_stream >> std::ws;
  std::string rest;
  std::getline(line_stream, rest);

  return std::make_tuple(*lower, rest);
}

}

