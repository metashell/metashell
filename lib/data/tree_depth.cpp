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

#include <metashell/data/tree_depth.hpp>

#include <iostream>
#include <stdexcept>

namespace metashell
{
  namespace data
  {
    tree_depth& tree_depth::operator++()
    {
      ++_value;
      return *this;
    }

    tree_depth& tree_depth::operator--()
    {
      assert(_value > 0);
      --_value;
      return *this;
    }

    tree_depth::operator int() const { return _value; }

    std::string to_string(tree_depth d_)
    {
      return "tree_depth(" + std::to_string(int(d_)) + ")";
    }

    std::ostream& operator<<(std::ostream& out_, tree_depth d_)
    {
      return out_ << to_string(d_);
    }

    void update(tree_depth& d_, const event_data& event_)
    {
      switch (relative_depth_of(event_))
      {
      case relative_depth::open:
        ++d_;
        break;
      case relative_depth::flat:
        break;
      case relative_depth::close:
      /* [[fallthrough]]; */ case relative_depth::end:
        if (int(d_) <= 0)
        {
          throw std::runtime_error("Unpaired closing event: " +
                                   to_string(event_));
        }
        --d_;
        break;
      }
    }
  }
}
