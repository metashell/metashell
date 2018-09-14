// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/includes.hpp>

namespace metashell
{
  namespace data
  {
    includes::includes(std::vector<boost::filesystem::path> sys_,
                       std::vector<boost::filesystem::path> quote_)
      : sys(move(sys_)), quote(move(quote_))
    {
    }

    const std::vector<boost::filesystem::path>& get(include_type type_,
                                                    const includes& includes_)
    {
      return type_ == include_type::sys ? includes_.sys : includes_.quote;
    }
  }
}
