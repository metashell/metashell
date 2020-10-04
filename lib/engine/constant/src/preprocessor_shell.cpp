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

#include <metashell/engine/constant/preprocessor_shell.hpp>

namespace metashell
{
  namespace engine
  {
    namespace constant
    {
      preprocessor_shell::preprocessor_shell(data::result result_)
        : _result(std::move(result_))
      {
      }

      data::result preprocessor_shell::precompile(const data::cpp_code&)
      {
        return _result;
      }
    } // namespace constant
  } // namespace engine
} // namespace metashell
