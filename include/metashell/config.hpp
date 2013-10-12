#ifndef METASHELL_CONFIG_HPP
#define METASHELL_CONFIG_HPP

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

#include <metashell/standard.hpp>

#include <string>
#include <vector>

namespace metashell
{
  class config
  {
  public:
    std::vector<std::string> include_path;
    bool verbose;
    bool syntax_highlight;
    bool indent;
    standard::type standard_to_use;

    static const config empty;
    static const config default_config;
  private:
    template <class InputIt>
    config(InputIt include_path_begin_, InputIt include_path_end_) :
      include_path(include_path_begin_, include_path_end_),
      verbose(false),
      syntax_highlight(true),
      indent(true),
      standard_to_use(standard::cpp11)
    {}
  };
}

#endif

