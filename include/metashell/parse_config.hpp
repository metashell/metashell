#ifndef METASHELL_PARSE_CONFIG_HPP
#define METASHELL_PARSE_CONFIG_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/user_config.hpp>
#include <metashell/engine_entry.hpp>

#include <iosfwd>
#include <string>
#include <map>

namespace metashell
{
  struct parse_config_result
  {
    enum class action_t
    {
      run_shell,
      exit_with_error,
      exit_without_error
    };

    action_t action;
    data::user_config cfg;

    bool should_run_shell() const;
    bool should_error_at_exit() const;

    static parse_config_result exit(bool with_error_);
    static parse_config_result start_shell(const data::user_config& cfg_);
  };

  parse_config_result parse_config(
    int argc_,
    const char* argv_[],
    const std::map<std::string, engine_entry>& engines_,
    std::ostream* out_ = 0,
    std::ostream* err_ = 0
  );

  std::ostream& operator<<(
    std::ostream& out_,
    parse_config_result::action_t a_
  );
}

#endif

