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

#include <metashell/exception.hpp>
#include <metashell/metaprogram_parse_trace.hpp>
#include <metashell/metaprogram_tracer_clang.hpp>

#include <metashell/data/stdin_name.hpp>

namespace metashell
{
  namespace
  {
    data::type_or_code_or_error type_or_code_or_error_from_result(
        const data::result& res_,
        const boost::optional<data::cpp_code>& expression_)
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

  metaprogram_tracer_clang::metaprogram_tracer_clang(clang_binary clang_binary_)
    : _clang_binary(clang_binary_)
  {
  }

  data::metaprogram metaprogram_tracer_clang::eval(
      iface::environment& env_,
      const boost::filesystem::path&,
      const boost::optional<data::cpp_code>& expression_,
      data::metaprogram_mode mode_,
      iface::displayer& displayer_)
  {
    const auto out = eval_with_templight_dump_on_stdout(
        env_, expression_, boost::none, _clang_binary);

    const data::result& res = std::get<0>(out);
    const std::string& trace = std::get<1>(out);

    if (!res.info.empty())
    {
      displayer_.show_raw_text(res.info);
    }

    if (trace.empty())
    {
      throw exception(res.successful ? "Failed to get template trace" :
                                       res.error);
    }
    else
    {
      const data::type_or_code_or_error evaluation_result =
          type_or_code_or_error_from_result(res, expression_);

      data::metaprogram result = create_metaprogram_from_yaml_trace(
          trace, mode_,
          expression_ ? *expression_ : data::cpp_code("<environment>"),
          evaluation_result, determine_from_line(env_.get(), expression_,
                                                 data::stdin_name_in_clang()));

      if (result.is_empty() && evaluation_result.is_error())
      {
        // Most errors will cause templight to generate an empty trace
        // We're only interested in non-empty traces
        throw exception(evaluation_result.get_error());
      }
      else
      {
        return result;
      }
    }
  }
}
