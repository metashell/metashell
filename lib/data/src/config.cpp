// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/config.hpp>
#include <metashell/data/exception.hpp>

#include <algorithm>
#include <optional>
#include <stdexcept>

namespace metashell
{
  namespace data
  {
    namespace
    {
      data::shell_config_name default_config()
      {
        return data::shell_config_name{"default"};
      }

      std::optional<std::vector<shell_config>::size_type>
      find_nothrow(const shell_config_name& name_,
                   const std::vector<shell_config>& configs_) noexcept
      {
        const auto begin = configs_.begin();
        const auto end = configs_.end();

        const auto i = std::find_if(
            begin, end,
            [&name_](const shell_config& c_) { return c_.name == name_; });
        if (i == end)
        {
          return std::nullopt;
        }
        else
        {
          return i - begin;
        }
      }

      std::vector<shell_config>::size_type
      find(const shell_config_name& name_,
           const std::vector<shell_config>& configs_) noexcept(false)
      {
        if (const auto i = find_nothrow(name_, configs_))
        {
          return *i;
        }
        else
        {
          throw exception{"Config " + name_ + " not found."};
        }
      }
    }

    config::config()
      : _shell_configs{shell_config{
            default_config(), data::shell_config_data{}}},
        _active_config{0}
    {
    }

    const std::vector<shell_config>& config::shell_configs() const
    {
      return _shell_configs;
    }

    shell_config& config::active_shell_config()
    {
      validate_active_config();
      return _shell_configs[_active_config];
    }

    const shell_config& config::active_shell_config() const
    {
      validate_active_config();
      return _shell_configs[_active_config];
    }

    shell_config& config::default_shell_config()
    {
      return _shell_configs[find(default_config(), _shell_configs)];
    }

    void config::activate(const shell_config_name& name_)
    {
      _active_config = find(name_, _shell_configs);
    }

    void config::push_back(shell_config config_)
    {
      if (find_nothrow(config_.name, _shell_configs))
      {
        throw std::runtime_error("Config " + config_.name + " already exists.");
      }
      else
      {
        _shell_configs.emplace_back(std::move(config_));
      }
    }

    void config::validate_active_config() const
    {
      if (_active_config >= _shell_configs.size())
      {
        throw std::runtime_error("No config has been selected");
      }
    }

    bool config::exists(const shell_config_name& name_) const
    {
      return bool(find_nothrow(name_, _shell_configs));
    }
  }
}
