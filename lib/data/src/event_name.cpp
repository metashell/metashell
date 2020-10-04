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

#include <metashell/data/event_name.hpp>

namespace metashell
{
  namespace data
  {
    namespace
    {
      template <class T>
      T to_metaprogram_node_impl(T t_)
      {
        return t_;
      }

      type to_metaprogram_node_impl(none_t) { return type(""); }
    } // namespace

    metaprogram_node to_metaprogram_node(const event_name& name_)
    {
      return mpark::visit(
          [](const auto& v) {
            return metaprogram_node(to_metaprogram_node_impl(v));
          },
          name_);
    }
  } // namespace data
} // namespace metashell
