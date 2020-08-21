#ifndef METASHELL_DATA_INCLUDE_CONFIG_HPP
#define METASHELL_DATA_INCLUDE_CONFIG_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/include_type.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <optional>
#include <vector>

namespace metashell
{
  namespace data
  {
    struct include_config : private boost::addable<include_config>
    {
      std::optional<boost::filesystem::path> isysroot;

      std::vector<boost::filesystem::path> iquote;
      std::vector<boost::filesystem::path> capital_i;
      std::vector<boost::filesystem::path> isystem;
      std::vector<boost::filesystem::path> idirafter;

      std::vector<boost::filesystem::path>
      get(include_type,
          const std::vector<boost::filesystem::path>& system_) const;

      include_config& operator+=(const include_config&);
    };

    command_line_argument_list to_clang_arguments(const include_config&);
    command_line_argument_list to_wave_arguments(const include_config&);
  }
}

#endif
