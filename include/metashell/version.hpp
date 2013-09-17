#ifndef METASHELL_VERSION_HPP
#define METASHELL_VERSION_HPP

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

#include <string>

#ifdef METASHELL_MAJOR
  #error METASHELL_MAJOR already defined
#endif
#ifdef METASHELL_MINOR
  #error METASHELL_MINOR already defined
#endif
#ifdef METASHELL_PATCH
  #error METASHELL_PATCH already defined
#endif

#define METASHELL_MAJOR 0
#define METASHELL_MINOR 1
#define METASHELL_PATCH 0

namespace metashell
{
  std::string wave_version();
  std::string libclang_version();
}

#endif

