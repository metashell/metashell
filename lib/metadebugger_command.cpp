
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

#include <metashell/metadebugger_command.hpp>

namespace metashell {

metadebugger_command::metadebugger_command(
    const keys_t& keys,
    repeatable_t rep,
    function func,
    const std::string& usage,
    const std::string& short_description,
    const std::string& long_description) :
  keys(keys), rep(rep), func(func), usage(usage),
  short_description(short_description), long_description(long_description)
{}

const metadebugger_command::keys_t& metadebugger_command::get_keys() const {
  return keys;
}

bool metadebugger_command::is_repeatable() const {
  return rep == repeatable;
}

metadebugger_command::function metadebugger_command::get_func() const {
  return func;
}

const std::string& metadebugger_command::get_usage() const {
  return usage;
}

const std::string& metadebugger_command::get_short_description() const {
  return short_description;
}

const std::string& metadebugger_command::get_long_description() const {
  return long_description;
}

std::string metadebugger_command::get_full_description() const {
  if (long_description.empty()) {
    return short_description;
  }
  return short_description + "\n" + long_description;
}

}

