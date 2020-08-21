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
#include <metashell/engine/wave/header_discoverer.hpp>

#include <metashell/data/unsupported_standard_headers_allowed.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      header_discoverer::header_discoverer(
          data::wave_config config_,
          std::vector<boost::filesystem::path> system_includes_)
        : _config(std::move(config_)),
          _system_includes(std::move(system_includes_))
      {
      }

      std::vector<boost::filesystem::path>
      header_discoverer::include_path(data::include_type type_,
                                      data::standard_headers_allowed allowed_)
      {
        if (allowed_ == data::standard_headers_allowed::none)
        {
          return {};
        }

        const std::vector<boost::filesystem::path> result =
            _config.config.includes.get(type_, _system_includes);
        if (allowed_ == data::standard_headers_allowed::all || result.empty())
        {
          return result;
        }
        else
        {
          throw data::unsupported_standard_headers_allowed(
              data::real_engine_name::wave, allowed_);
        }
      }

      std::set<boost::filesystem::path>
      header_discoverer::files_included_by(const data::cpp_code& exp_)
      {
        const data::cpp_code exp = exp_ + data::cpp_code("\n");
        std::set<boost::filesystem::path> result;
        hooks hks(result);
        context ctx(exp.begin(), exp.end(), "<stdin>", hks);
        wave::apply(ctx, _config, _system_includes);
        preprocess(ctx);
        return result;
      }
    }
  }
}
