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

#include <metashell/core/type_shell_constant.hpp>

namespace metashell
{
  type_shell_constant::type_shell_constant(data::result result_)
    : _result(std::move(result_))
  {
  }

  data::result type_shell_constant::eval(const iface::environment&,
                                         const boost::optional<data::cpp_code>&,
                                         bool)
  {
    return _result;
  }

  void type_shell_constant::generate_precompiled_header(
      const boost::filesystem::path&)
  {
    // ignore
  }
}
