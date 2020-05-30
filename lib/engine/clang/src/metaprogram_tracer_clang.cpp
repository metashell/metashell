// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/clang/metaprogram_tracer.hpp>
#include <metashell/engine/clang/yaml_trace.hpp>

#include <metashell/data/exception.hpp>

#include <metashell/core/filter_events.hpp>

#include <metashell/data/stdin_name.hpp>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        data::type_or_code_or_error type_or_code_or_error_from_result(
            const data::result& res_,
            const std::optional<data::cpp_code>& expression_)
        {
          if (res_.successful)
          {
            return expression_ ? data::type_or_code_or_error::make_type(
                                     data::type(res_.output)) :
                                 data::type_or_code_or_error::make_none();
          }
          else
          {
            return data::type_or_code_or_error::make_error(res_.error);
          }
        }
      }

      metaprogram_tracer::metaprogram_tracer(binary binary_) : _binary(binary_)
      {
      }

      std::unique_ptr<iface::event_data_sequence>
      metaprogram_tracer::eval(iface::environment& env_,
                               const boost::filesystem::path&,
                               const std::optional<data::cpp_code>& expression_,
                               data::metaprogram_mode mode_,
                               iface::displayer& displayer_)
      {
        const auto out = eval_with_templight_dump_on_stdout(
            env_, expression_, std::nullopt, _binary, mode_);

        const data::result& res = std::get<0>(out);
        const std::string& trace = std::get<1>(out);

        if (!res.info.empty())
        {
          displayer_.show_raw_text(res.info);
        }

        if (trace.empty())
        {
          throw data::exception(
              res.successful ? "Failed to get template trace" : res.error);
        }
        else
        {
          return core::filter_events(
              yaml_trace(
                  trace, type_or_code_or_error_from_result(res, expression_),
                  expression_ ? *expression_ : data::cpp_code("<environment>"),
                  mode_),
              determine_from_line(
                  env_.get(), expression_, data::stdin_name_in_clang()));
        }
      }
    }
  }
}
