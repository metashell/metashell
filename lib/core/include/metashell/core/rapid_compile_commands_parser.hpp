#ifndef METASHELL_RAPID_COMPILE_COMMANDS_PARSER_HPP
#define METASHELL_RAPID_COMPILE_COMMANDS_PARSER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <optional>
#include <vector>

namespace metashell
{
  namespace core
  {
    class rapid_compile_commands_parser
        : public rapid_handler<rapid_compile_commands_parser, true>
    {
    public:
      rapid_compile_commands_parser(bool use_precompiled_headers_,
                                    bool preprocessor_mode_);

      bool StartArray();
      bool end_array();

      bool StartObject();
      bool end_object();

      bool key(const std::string& str_);

      bool Bool(bool b_);
      bool string(const std::string& str_);

      const std::vector<data::shell_config>& configs() const;

    private:
      data::shell_config_data _defaults;

      std::vector<data::shell_config> _configs;

      bool _in_list = false;
      std::optional<data::shell_config_name> _name = std::nullopt;
      std::optional<data::shell_config_data> _data = std::nullopt;
      std::optional<std::string> _key = std::nullopt;
    };
  }
}

#endif
