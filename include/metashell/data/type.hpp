#ifndef METASHELL_TYPE_HPP
#define METASHELL_TYPE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <string>
#include <iosfwd>

namespace metashell
{
  namespace data
  {
    class type : boost::totally_ordered<type>
    {
    public:
      type();
      explicit type(const std::string& name_);

      const std::string& name() const;

      bool is_integral_constant(const type& type_,
                                const std::string& value_) const;

    private:
      std::string _name;
    };

    std::ostream& operator<<(std::ostream& o_, const type& t_);
    bool operator==(const type& a_, const type& b_);
    bool operator<(const type& a_, const type& b_);
  }
}

#endif
