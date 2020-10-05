#ifndef MDB_COMMAND_HPP
#define MDB_COMMAND_HPP

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

#include <metashell/mdb/repeatable_t.hpp>

#include <metashell/iface/displayer.hpp>

#include <metashell/data/mdb_command.hpp>

#include <functional>
#include <string>
#include <vector>

namespace metashell
{
  namespace mdb
  {
    class shell;

    class command
    {
    public:
      typedef std::vector<data::mdb_command::name_type> keys_t;
      typedef std::function<void(
          shell&, const data::mdb_command::arguments_type&, iface::displayer&)>
          function;

      command() = default;
      command(const keys_t& key,
              repeatable_t rep,
              function func,
              const std::string& usage,
              const std::string& short_description,
              const std::string& long_description);

      const keys_t& get_keys() const;
      bool is_repeatable() const;
      function get_func() const;
      const std::string& get_usage() const;
      const std::string& get_short_description() const;
      const std::string& get_long_description() const;
      std::string get_full_description() const;

    private:
      keys_t keys;
      repeatable_t rep;
      function func;
      std::string usage;
      std::string short_description;
      std::string long_description;
    };
  } // namespace mdb
} // namespace metashell

#endif
