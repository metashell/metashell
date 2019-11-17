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

#include <metashell/data/engine_config.hpp>

namespace metashell
{
  namespace data
  {
    namespace
    {
      void append(std::vector<boost::filesystem::path>& to_,
                  const std::vector<boost::filesystem::path>& items_)
      {
        to_.insert(to_.end(), items_.begin(), items_.end());
      }
    }

    std::vector<boost::filesystem::path> include_config::get(
        include_type type_,
        const std::vector<boost::filesystem::path>& system_) const
    {
      std::vector<boost::filesystem::path> result;

      switch (type_)
      {
      case include_type::sys:
        break;
      case include_type::quote:
        append(result, iquote);
        break;
      }

      append(result, capital_i);
      append(result, isystem);
      append(result, system_);
      append(result, idirafter);

      return result;
    }

    include_config& include_config::operator+=(const include_config& rhs_)
    {
      append(iquote, rhs_.iquote);
      append(capital_i, rhs_.capital_i);
      append(isystem, rhs_.isystem);
      append(idirafter, rhs_.idirafter);
      return *this;
    }
  }
}
