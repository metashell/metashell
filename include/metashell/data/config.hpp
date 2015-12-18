#ifndef METASHELL_DATA_CONFIG_HPP
#define METASHELL_DATA_CONFIG_HPP

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
#include <vector>

namespace metashell
{
  namespace data
  {
    class config
    {
    public:
      bool verbose = false;
      bool warnings_enabled = true;
      std::vector<std::string> extra_clang_args;
      bool use_precompiled_headers = false;
      std::string clang_path;
      int max_template_depth;
      unsigned templight_trace_capacity;
      bool saving_enabled;
      bool splash_enabled = true;
    };
  }
}

#endif

