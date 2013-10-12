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

#include <metashell/config.hpp>

using namespace metashell;

namespace
{
  const char* extra_sysinclude[] =
    {
      ""
      #include "extra_sysinclude.hpp"
    };
}

const config config::empty((const char**)0, (const char**)0);

const config
  config::default_config(
    extra_sysinclude + 1,
    extra_sysinclude + sizeof(extra_sysinclude) / sizeof(const char*)
  );


