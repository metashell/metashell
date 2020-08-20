
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

#include <metashell/mdb/command_handler_map.hpp>
#include <metashell/mdb/shell.hpp>

namespace metashell
{
  namespace mdb
  {
    command_handler_map::command_handler_map(const commands_t& commands_arg)
      : commands(commands_arg)
    {
      std::vector<key_command_map_t::value_type> key_command_vec;
      for (std::size_t i = 0; i < commands.size(); ++i)
      {
        for (const data::mdb_command::name_type& key : commands[i].get_keys())
        {
          key_command_vec.push_back(key_command_map_t::value_type(key, i));
        }
      }
      key_command_map.insert(key_command_vec.begin(), key_command_vec.end());
    }

    std::optional<command> command_handler_map::get_command(
        const data::mdb_command::name_type& name) const
    {
      if (name.empty())
      {
        return std::nullopt;
      }

      key_command_map_t::const_iterator lower =
          key_command_map.lower_bound(name);

      if (lower == key_command_map.end() || !starts_with(lower->first, name))
      {
        return std::nullopt;
      }

      // Check if the found command is unambiguous
      if (name != lower->first)
      { // Pass if the match is full
        std::size_t command_index = lower->second;
        for (key_command_map_t::const_iterator it = std::next(lower);
             it != key_command_map.end() && starts_with(it->first, name); ++it)
        {
          // Even there are multiple matches,
          // aliases for the same command should pass
          // For example forwardtrace and ft with line "f"
          if (it->second != command_index)
          {
            return std::nullopt;
          }
        }
      }

      assert(lower->second < commands.size());
      return commands[lower->second];
    }

    const command_handler_map::commands_t&
    command_handler_map::get_commands() const
    {
      return commands;
    }
  }
}
