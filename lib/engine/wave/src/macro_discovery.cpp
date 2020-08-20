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

#include <metashell/engine/wave/context.hpp>
#include <metashell/engine/wave/macro_discovery.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      macro_discovery::macro_discovery(
          data::wave_config config_,
          std::vector<boost::filesystem::path> system_includes_)
        : _config(std::move(config_)),
          _system_includes(std::move(system_includes_))
      {
      }

      data::cpp_code macro_discovery::macros(const iface::environment& env_)
      {
        const data::cpp_code code = env_.get_all() + data::cpp_code("\n");
        context ctx(code.begin(), code.end(), "<stdin>");
        wave::apply(ctx, _config, _system_includes);
        preprocess(ctx);

        std::ostringstream result;

        const auto e = ctx.macro_names_end();
        for (auto i = ctx.macro_names_begin(); i != e; ++i)
        {
          result << "#define " << *i;

          bool function_style = false;
          bool predefined = false;
          context::position_type pos;
          std::vector<core::wave_token> parameters;
          context::token_sequence_type definition;

          if (ctx.get_macro_definition(
                  *i, function_style, predefined, pos, parameters, definition))
          {
            if (function_style)
            {
              result << "(";
              display(result, parameters, _config.ignore_macro_redefinition);
              result << ") ";
            }
            else
            {
              result << " ";
            }
            display(result, definition, _config.ignore_macro_redefinition);
          }
          result << "\n";
        }

        return data::cpp_code(result.str());
      }
    }
  }
}
