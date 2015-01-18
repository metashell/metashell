// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/version.hpp>
#include <metashell/to_string.hpp>

using namespace metashell;

std::string metashell::version()
{
#ifndef METASHELL_MAJOR
  #error METASHELL_MAJOR not defined
#endif

#ifndef METASHELL_MINOR
  #error METASHELL_MINOR not defined
#endif

#ifndef METASHELL_PATCH
  #error METASHELL_PATCH not defined
#endif

  return
    TO_STRING(METASHELL_MAJOR)
    "." TO_STRING(METASHELL_MINOR)
    "." TO_STRING(METASHELL_PATCH);
}


