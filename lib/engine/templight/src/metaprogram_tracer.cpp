// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/templight/metaprogram_tracer.hpp>
#include <metashell/engine/templight/protobuf_trace.hpp>

#include <metashell/data/stdin_name.hpp>

#include <metashell/core/filter_events.hpp>

namespace metashell
{
  namespace engine
  {
    namespace templight
    {
      namespace
      {
        data::type_or_code_or_error
        run_metaprogram(clang::binary& clang_binary_,
                        const std::optional<data::cpp_code>& expression_,
                        const boost::filesystem::path& output_path_,
                        iface::environment& env_,
                        iface::displayer& displayer_)
        {
          using data::type_or_code_or_error;

          const data::result res = eval(
              env_, expression_, std::nullopt, output_path_, clang_binary_);

          if (!res.info.empty())
          {
            displayer_.show_raw_text(res.info);
          }

          if (!res.successful)
          {
            return type_or_code_or_error::make_error(res.error);
          }
          else if (expression_)
          {
            return type_or_code_or_error::make_type(data::type(res.output));
          }
          else
          {
            return type_or_code_or_error::make_none();
          }
        }
      }

      metaprogram_tracer::metaprogram_tracer(clang::binary templight_binary_)
        : _templight_binary(templight_binary_)
      {
      }

      std::unique_ptr<iface::event_data_sequence>
      metaprogram_tracer::eval(iface::environment& env_,
                               const boost::filesystem::path& temp_dir_,
                               const std::optional<data::cpp_code>& expression_,
                               data::metaprogram_mode mode_,
                               iface::displayer& displayer_)
      {
        const boost::filesystem::path output_path = temp_dir_ / "templight.pb";

        const data::type_or_code_or_error evaluation_result = run_metaprogram(
            _templight_binary, expression_, output_path, env_, displayer_);

        return core::filter_events(
            protobuf_trace(
                output_path.string() + ".trace.pbf", evaluation_result,
                expression_ ? *expression_ : data::cpp_code("<environment>"),
                mode_),
            determine_from_line(
                env_.get(), expression_, data::stdin_name_in_clang()));
      }
    }
  }
}
