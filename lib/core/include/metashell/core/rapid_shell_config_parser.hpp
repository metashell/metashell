#ifndef METASHELL_RAPID_SHELL_CONFIG_PARSER_HPP
#define METASHELL_RAPID_SHELL_CONFIG_PARSER_HPP

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

#include <metashell/data/shell_config.hpp>

#include <metashell/core/rapid_handler.hpp>

#include <functional>
#include <optional>

namespace metashell
{
  namespace core
  {
    class rapid_shell_config_parser
        : public rapid_handler<rapid_shell_config_parser, true>
    {
    public:
      std::function<void(data::shell_config)> parsed_config_callback =
          [](data::shell_config) { /* throw away */ };

      bool StartArray();
      bool end_array();

      bool StartObject();
      bool end_object();

      bool key(const std::string& str_);

      bool Bool(bool b_);
      bool string(const std::string& str_);

    private:
      bool _in_main_list = false;
      bool _in_value_list = false;
      std::optional<data::shell_config_name> _name = std::nullopt;
      std::optional<data::shell_config_data> _data = std::nullopt;
      std::optional<std::string> _key = std::nullopt;
    };
  }
}

#endif
