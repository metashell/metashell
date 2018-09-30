#ifndef METASHELL_DATA_TREE_DEPTH_HPP
#define METASHELL_DATA_TREE_DEPTH_HPP

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

#include <metashell/data/event_data.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class tree_depth : boost::unit_steppable<tree_depth>
    {
    public:
      tree_depth& operator++();
      tree_depth& operator--();

      explicit operator int() const;

    private:
      int _value = 0;
    };

    void update(tree_depth& d_, const event_data& event_);

    std::string to_string(tree_depth d_);
    std::ostream& operator<<(std::ostream& out_, tree_depth d_);
  }
}

#endif
