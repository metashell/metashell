// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/kind_of_mp.hpp>
#include <metashell/data/timeless_event_data.hpp>

#include <variant>

namespace metashell
{
  namespace data
  {
    event_kind kind_of(const timeless_event_data& data)
    {
      return visit([](const auto& details) { return kind_of(details); }, data);
    }

    bool operator==(const timeless_event_data& a, const timeless_event_data& b)
    {
      return kind_of(a) == kind_of(b) &&
             std::visit(
                 [&b](const auto& da) -> bool {
                   return da == std::get<timeless_event_details<
                                    kind_of_mp<decltype(da)>::value>>(b);
                 },
                 a);
    }
  } // namespace data
} // namespace metashell
