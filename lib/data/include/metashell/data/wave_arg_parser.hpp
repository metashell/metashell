#ifndef METASHELL_DATA_WAVE_ARG_PARSER_HPP
#define METASHELL_DATA_WAVE_ARG_PARSER_HPP

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

#include <metashell/data/arg_parser.hpp>
#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/include_config.hpp>
#include <metashell/data/wave_config.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class wave_arg_parser
    {
    public:
      explicit wave_arg_parser(bool use_templight_headers_);

      wave_arg_parser(const wave_arg_parser&) = delete;
      wave_arg_parser(wave_arg_parser&&) = delete;

      wave_arg_parser& operator=(const wave_arg_parser&) = delete;
      wave_arg_parser& operator=(wave_arg_parser&&) = delete;

      void parse(const command_line_argument_list& args_,
                 std::vector<macro_definition> sysmacros_);

      const wave_config& result() const;

      std::string description(int console_width_) const;

    private:
      data::include_config _includes;
      std::vector<command_line_argument> _standards;

      wave_config _config;

      arg_parser _parser;
    };
  }
}

#endif
