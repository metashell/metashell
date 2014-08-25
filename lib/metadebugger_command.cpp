
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
    const std::string& key,
    function func) :
  key(key), func(func)
{}

const std::string& metadebugger_command::get_key() const {
  return key;
}

metadebugger_command::function metadebugger_command::get_func() const {
  return func;
}

bool operator<(
    const metadebugger_command& lhs,
    const metadebugger_command& rhs)
{
  return lhs.get_key() < rhs.get_key();
}

bool operator<(
    const metadebugger_command& lhs,
    const std::string& rhs)
{
  return lhs.get_key() < rhs;
}

}

