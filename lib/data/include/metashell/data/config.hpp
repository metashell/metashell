#ifndef METASHELL_DATA_CONFIG_HPP
#define METASHELL_DATA_CONFIG_HPP

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

#include <metashell/data/console_type.hpp>
#include <metashell/data/executable_path.hpp>
#include <metashell/data/logging_mode.hpp>
#include <metashell/data/shell_config.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class config
    {
    public:
      bool verbose = false;
      bool syntax_highlight = true;
      bool indent = true;
      bool saving_enabled = true;
      console_type con_type = console_type::plain;
      bool splash_enabled = true;
      logging_mode log_mode = logging_mode::none;
      std::string log_file;

      config();

      const std::vector<shell_config>& shell_configs() const;

      shell_config& default_shell_config();

      shell_config& active_shell_config();
      const shell_config& active_shell_config() const;

      void activate(const shell_config_name& name_);

      void push_back(shell_config config_);

      bool exists(const shell_config_name& name_) const;

    private:
      std::vector<shell_config> _shell_configs;

      // using index instead of iterator to avoid it getting invalidated
      // during insertion.
      std::vector<shell_config>::size_type _active_config;

      void validate_active_config() const;
    };
  }
}

#endif
