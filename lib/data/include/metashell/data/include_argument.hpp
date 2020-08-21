#ifndef METASHELL_DATA_INCLUDE_ARGUMENT_HPP
#define METASHELL_DATA_INCLUDE_ARGUMENT_HPP

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

#include <metashell/data/command.hpp>
#include <metashell/data/include_type.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <iosfwd>
#include <map>
#include <optional>

namespace metashell
{
  namespace data
  {
    struct include_argument : boost::totally_ordered<include_argument>
    {
      include_argument(include_type type_,
                       const boost::filesystem::path& path_);
      include_type type;
      boost::filesystem::path path;

      static std::pair<std::optional<include_argument>, command::iterator>
      parse(const command::iterator& begin_, const command::iterator& end_);
    };

    std::ostream& operator<<(std::ostream& out_, const include_argument& args_);
    bool operator==(const include_argument& a_, const include_argument& b_);
    bool operator<(const include_argument& a_, const include_argument& b_);

    std::string include_code(const include_argument& arg_);
  }
}

#endif
