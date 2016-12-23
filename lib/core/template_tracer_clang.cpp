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

#include <metashell/template_tracer_clang.hpp>

namespace metashell
{
  template_tracer_clang::template_tracer_clang(
      const boost::filesystem::path& internal_dir_,
      const boost::filesystem::path& env_filename_,
      clang_binary clang_binary_)
    : _clang_binary(clang_binary_), _env_path(internal_dir_ / env_filename_)
  {
  }

  data::result template_tracer_clang::eval(
      const iface::environment& env_,
      const boost::optional<std::string>& tmp_exp_,
      bool use_precompiled_headers_,
      const boost::filesystem::path& templight_dump_path_)
  {
    return metashell::eval(
        env_, tmp_exp_,
        use_precompiled_headers_ ?
            boost::optional<boost::filesystem::path>(_env_path) :
            boost::none,
        templight_dump_path_, _clang_binary);
  }
}
