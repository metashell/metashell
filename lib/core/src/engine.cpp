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

#include <metashell/core/engine.hpp>

namespace metashell
{
  namespace core
  {
    namespace
    {
      template <class Feature>
      bool supports(const iface::engine& engine_)
      {
        try
        {
          iface::get<Feature>(engine_);
          return true;
        }
        catch (const data::some_feature_not_supported&)
        {
          return false;
        }
      }

      // Unspecified result for unrealistic cases (eg. type_tracer works but
      // type_shell does not).
      std::optional<std::string> limitation_impl(const iface::engine& engine_)
      {
        if (supports<iface::type_shell>(engine_))
        {
          if (supports<iface::preprocessor_tracer>(engine_))
          {
            return std::nullopt;
          }
          else
          {
            return std::string("not use the preprocessor tracer");
          }
        }
        else if (supports<iface::preprocessor_shell>(engine_))
        {
          std::string msg =
              "not debug types or metaprograms, only the preprocessor";
          if (!supports<iface::preprocessor_tracer>(engine_))
          {
            msg += " (but not trace it";
            if (!supports<iface::macro_discovery>(engine_))
            {
              msg += " or query the defined macros";
            }
            msg += ")";
          }
          return msg;
        }
        else
        {
          return std::string("not debug anything");
        }
      }
    }

    std::optional<std::string> limitation(const iface::engine& engine_)
    {
      if (const auto limit = limitation_impl(engine_))
      {
        return "Note that you are using the " + engine_.name() +
               " engine, which means that you can " + *limit +
               ". Metashell supports different features if you use different "
               "compilers.";
      }
      else
      {
        return std::nullopt;
      }
    }
  }
}
