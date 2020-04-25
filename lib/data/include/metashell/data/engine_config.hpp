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

#include <metashell/data/clang_arch.hpp>
#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/engine_arguments.hpp>
#include <metashell/data/include_config.hpp>
#include <metashell/data/language_standard.hpp>
#include <metashell/data/macro_def_or_undef.hpp>
#include <metashell/data/standard_headers_allowed.hpp>

#include <vector>

namespace metashell
{
  namespace data
  {
    struct engine_config
    {
      include_config includes;

      std::vector<macro_def_or_undef> macros;
      language_standard standard = language_standard::cpp98;
      standard_headers_allowed use_standard_headers =
          standard_headers_allowed::all;
      int template_instantiation_depth = 900;
      bool warnings_enabled = true;
      std::vector<clang_arch> archs;
    };

    engine_arguments convert_to(real_engine_name, const engine_config&);

    engine_config parse_vc_arguments(const command_line_argument_list&);

    engine_config parse_clang_arguments(const command_line_argument_list&);
  }
}

#endif
