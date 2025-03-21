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

#include <metashell/engine/constant/cpp_validator.hpp>

namespace metashell
{
  namespace engine
  {
    namespace constant
    {
      cpp_validator::cpp_validator(data::result result_)
        : _result(std::move(result_))
      {
      }

      data::result cpp_validator::validate_code(const data::cpp_code&,
                                                const data::config&,
                                                const iface::environment&,
                                                bool)
      {
        return _result;
      }
    } // namespace constant
  } // namespace engine
} // namespace metashell
