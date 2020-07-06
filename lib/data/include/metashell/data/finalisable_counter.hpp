#ifndef METASHELL_DATA_FINALISABLE_COUNTER_HPP
#define METASHELL_DATA_FINALISABLE_COUNTER_HPP

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

#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <string>

namespace metashell
{
  namespace data
  {
    class finalisable_counter : boost::incrementable<finalisable_counter>
    {
    public:
      // precondition: !_finalised
      finalisable_counter& operator++();

      // precondition: !_finalised
      void value_is_final();

      std::optional<int> operator*() const;

      int current_value() const;

    private:
      int _value = 0;
      bool _final = false;
    };

    std::string to_string(const finalisable_counter& c_);
    std::ostream& operator<<(std::ostream& out_, const finalisable_counter& c_);
  }
}

#endif
