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

#include <metashell/data/engine_config.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/unsupported_standard_headers_allowed.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>
#include <cassert>

namespace metashell
{
  namespace data
  {
    namespace
    {
      command_line_argument_list to_clang_arguments(const engine_config& cfg_)
      {
        command_line_argument_list result = to_clang_arguments(cfg_.includes);

        for (const auto& macro : cfg_.macros)
        {
          result.push_back(clang_argument(macro));
        }

        result.push_back(command_line_argument("-std=") +
                         gcc_name(cfg_.standard));

        switch (cfg_.use_standard_headers)
        {
        case standard_headers_allowed::none:
          result.push_back("-nostdinc");
          result.push_back("-nostdinc++");
          break;
        case standard_headers_allowed::c:
          result.push_back("-nostdinc");
          break;
        case standard_headers_allowed::cpp:
          result.push_back("-nostdinc++");
          break;
        case standard_headers_allowed::all:
          break;
        }

        return result;
      }

      command_line_argument_list to_wave_arguments(const engine_config& cfg_)
      {
        command_line_argument_list result = to_wave_arguments(cfg_.includes);

        for (const auto& macro : cfg_.macros)
        {
          result.push_back(clang_argument(macro));
        }

        result.push_back(wave_name(cfg_.standard));

        switch (cfg_.use_standard_headers)
        {
        case standard_headers_allowed::none:
          result.push_back("--nostdinc++");
          break;
        case standard_headers_allowed::c:
        case standard_headers_allowed::cpp:
          throw unsupported_standard_headers_allowed(
              real_engine_name::wave, cfg_.use_standard_headers);
          break;
        case standard_headers_allowed::all:
          break;
        }

        return result;
      }

      command_line_argument_list to_arguments(real_engine_name engine_,
                                              const engine_config& cfg_)
      {
        switch (engine_)
        {
        case real_engine_name::internal:
          return to_clang_arguments(cfg_);
        case real_engine_name::pure_wave:
        case real_engine_name::wave:
          return to_wave_arguments(cfg_);
        case real_engine_name::null:
          return {};
        case real_engine_name::clang:
        case real_engine_name::gcc:
        case real_engine_name::msvc:
        case real_engine_name::templight:
          throw exception("Switching to engine " + to_string(engine_) +
                          " is not supported.");
        }
        assert(!"Invalid engine name");
        return {};
      }
    }

    engine_arguments convert_to(real_engine_name engine_,
                                const engine_config& cfg_)
    {
      return {to_arguments(engine_, cfg_), engine_};
    }
  }
}
