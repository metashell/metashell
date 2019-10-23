#ifndef METASHELL_DATA_ENGINE_CONFIG_HPP
#define METASHELL_DATA_ENGINE_CONFIG_HPP

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

#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/include_config.hpp>
#include <metashell/data/wave_standard.hpp>

#include <boost/optional.hpp>

#include <vector>

namespace metashell
{
  namespace data
  {
    struct engine_config
    {
      include_config includes;

      std::vector<std::string> macros;
      boost::optional<wave_standard> standard;
      bool use_standard_headers = true;
    };
  }
}

#endif
