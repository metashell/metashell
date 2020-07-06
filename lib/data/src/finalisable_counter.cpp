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

#include <metashell/data/finalisable_counter.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    finalisable_counter& finalisable_counter::operator++()
    {
      assert(!_final);
      ++_value;
      return *this;
    }

    void finalisable_counter::value_is_final()
    {
      assert(!_final);
      _final = true;
    }

    std::optional<int> finalisable_counter::operator*() const
    {
      return _final ? std::make_optional(_value) : std::nullopt;
    }

    int finalisable_counter::current_value() const { return _value; }

    std::string to_string(const finalisable_counter& c_)
    {
      if (const std::optional<int> v = *c_)
      {
        return std::to_string(*v) + " (final)";
      }
      else
      {
        return std::to_string(c_.current_value()) + " (not final)";
      }
    }

    std::ostream& operator<<(std::ostream& out_, const finalisable_counter& c_)
    {
      return out_ << to_string(c_);
    }
  }
}
