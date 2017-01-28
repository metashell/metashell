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

#include <fstream>

namespace
{
  metashell::data::type_or_error
  run_metaprogram(metashell::clang_binary& clang_binary_,
                  const boost::optional<metashell::data::cpp_code>& expression_,
                  const boost::filesystem::path& output_path_,
                  const boost::filesystem::path& env_path_,
                  bool use_precompiled_headers_,
                  metashell::iface::environment& env_,
                  metashell::iface::displayer& displayer_)
  {
    using metashell::data::type_or_error;

    const metashell::data::result res = metashell::eval(
        env_, expression_,
        use_precompiled_headers_ ?
            boost::optional<boost::filesystem::path>(env_path_) :
            boost::none,
        output_path_, clang_binary_);

    if (!res.info.empty())
    {
      displayer_.show_raw_text(res.info);
    }

    if (!res.successful)
    {
      return type_or_error::make_error(res.error);
    }
    else if (expression_)
    {
      return type_or_error::make_type(metashell::data::type(res.output));
    }
    else
    {
      return type_or_error::make_none();
    }
  }
}

namespace metashell
{
  metaprogram_tracer_clang::metaprogram_tracer_clang(
      const boost::filesystem::path& internal_dir_,
      const boost::filesystem::path& env_filename_,
      clang_binary clang_binary_)
    : _clang_binary(clang_binary_), _env_path(internal_dir_ / env_filename_)
  {
  }

  data::metaprogram metaprogram_tracer_clang::eval(
      iface::environment& env_,
      const boost::filesystem::path& temp_dir_,
      const boost::optional<data::cpp_code>& expression_,
      data::metaprogram::mode_t mode_,
      bool use_precompiled_headers_,
      iface::displayer& displayer_)
  {
    const boost::filesystem::path output_path = temp_dir_ / "templight.pb";

    const data::type_or_error evaluation_result =
        run_metaprogram(_clang_binary, expression_, output_path, _env_path,
                        use_precompiled_headers_, env_, displayer_);

    // Opening in binary mode, because some platforms interpret some characters
    // specially in text mode, which caused parsing to fail.
    std::ifstream protobuf_stream(output_path.string() + ".trace.pbf",
                                  std::ios_base::in | std::ios_base::binary);

    if (protobuf_stream)
    {
      const data::metaprogram result = create_metaprogram_from_protobuf_stream(
          protobuf_stream, mode_,
          expression_ ? *expression_ : data::cpp_code("<environment>"),
          data::file_location{}, // TODO something sensible here?
          evaluation_result);
      if (result.is_empty() && evaluation_result.is_error())
      {
        // Most errors will cause templight to generate an empty trace
        // We're only intrested in non-empty traces
        throw exception(evaluation_result.get_error());
      }
      return result;
    }
    else if (evaluation_result.is_error())
    {
      throw exception(evaluation_result.get_error());
    }
    else
    {
      // Shouldn't happen
      throw exception("Unexpected type type_or_error result");
    }
  }
}
