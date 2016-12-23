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

#include <metashell/template_tracer_constant.hpp>

namespace metashell
{
  template_tracer_constant::template_tracer_constant(data::result result_)
    : _result(std::move(result_))
  {
  }

  data::result
  template_tracer_constant::eval(const iface::environment&,
                                 const boost::optional<std::string>&,
                                 bool,
                                 const boost::filesystem::path&)
  {
    return _result;
  }
}
